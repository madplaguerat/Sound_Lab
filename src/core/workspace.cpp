#include "audio.h"

using namespace Audio;

Workspace::Workspace(MainWindow *parent_window)
{
    this->device = alcOpenDevice(NULL);
    this->context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    this->parent_window = parent_window;
    this->tempo = 120;
    this->time_signature = std::make_pair(4, 4);

    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listener_ori);

    ALuint main_source;
    ALuint main_buf;
    int *aux_data = new int[default_track_len * default_frequency];
    memset(aux_data, 0, default_track_len * default_frequency);
    alGenBuffers(1, &main_buf);
    alBufferData(main_buf, AL_FORMAT_MONO16, aux_data,
                 default_track_len * default_frequency,
            default_frequency);

    alGenSources(1, &main_source);
    init_source(main_source, main_buf, 0, 0, 0);
    source_vec.push_back(main_source);
    is_playing = false;

    play_tread = new PlayThread(this, this->parent_window);
    delete [] aux_data;

    check_errors();
}

Workspace::~Workspace()
{
}

QStringList Workspace::list_capture_devices()
{
    const ALchar *devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    QStringList capt_lst;
    const ALCchar *device = devices, *next = devices + 1;
    std::size_t len = 0;

    while (device && *device != '\0' && next && *next != '\0')
    {
        capt_lst << device;
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }

    return capt_lst;
}

void Workspace::play()
{
    alSourcePlayv((ALsizei)source_vec.size(), source_vec.data());
    is_playing = true;
    play_tread->start(QThread::LowPriority);
}

void Workspace::pause()
{
    alSourcePausev((ALsizei)source_vec.size(), source_vec.data());
    is_playing = false;
}

void Workspace::stop()
{
    alSourceStopv((ALsizei)source_vec.size(), source_vec.data());
    is_playing = false;
    this->curr_offset = 0;

}

void Workspace::add_track()
{
    std::string newbie_name = "Track_" + std::to_string(this->tracks_cnt());
    Audio::Track *newbie = new Audio::Track(this,
                            newbie_name,
                            default_track_len * default_frequency, default_frequency);
    track_source.insert(std::pair<Track*, int>(newbie, 2 * (tracks.size()) + 1));

    ALuint *buff_source = new ALuint [2];

    alGenSources((ALuint)2, buff_source);

    qDebug() << buff_source[0] << " " << buff_source[1];

    init_source(buff_source[0], std::get<0>(newbie->get_buffs()), 1, 0, 0);
    init_source(buff_source[1], std::get<1>(newbie->get_buffs()), -1, 0, 0);
    source_vec.push_back(buff_source[0]);
    source_vec.push_back(buff_source[1]);

    tracks.push_back(newbie);
}

void Workspace::delete_track()
{

}

int Workspace::tracks_cnt()
{
    return this->tracks.size();
}

void Workspace::init_source(ALuint src, ALuint buff, int x, int y, int z)
{
    alSourcef(src, AL_PITCH, 1);
    alSourcef(src, AL_GAIN, 1);
    alSource3f(src, AL_POSITION, x, y, z);
    alSource3f(src, AL_VELOCITY, 0, 0, 0);
    alSourcei(src, AL_LOOPING, AL_FALSE);
    alSourcei(src, AL_BUFFER, buff);
}

void Workspace::init_source(Track *t)
{
    ALuint l_source = this->source_vec[this->track_source[t]];
    ALuint r_source = l_source + 1;
    alSourcei(l_source, AL_BUFFER, t->get_buffs().first);
    alSourcei(r_source, AL_BUFFER, t->get_buffs().second);
    qDebug() << "Before";
    alSourcei(l_source, AL_SAMPLE_OFFSET, (ALuint)this->get_offset_playback(SAMPLES));
    alSourcei(r_source, AL_SAMPLE_OFFSET, (ALuint)this->get_offset_playback(SAMPLES));
    qDebug() << "After";
    check_errors();
}

void Workspace::unqueue_from_source(Track *t)
{
    ALuint l_source = this->source_vec[this->track_source[t]];
    ALuint r_source = l_source + 1;
    if(alIsSource(l_source))
        qDebug() << "L allright";
    if(alIsSource(r_source))
        qDebug() << "R allright";
    alSourcei(l_source, AL_BUFFER, NULL);
    alSourcei(r_source, AL_BUFFER, NULL);
    check_errors();
}

void Workspace::close_openal()
{
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void Workspace::check_errors()
{
    ALenum err_al = alGetError();
    ALenum err_alc = alcGetError(device);
    char* err_al_str = new char[30];
    err_al_str = (char*)alGetString(err_al);
    char* err_alc_str = new char[30];
    err_alc_str = (char*)alcGetString(device, err_alc);
    qDebug() << err_alc_str;
    qDebug() << err_al_str;
}

float Workspace::get_offset_playback(int type)
{
    ALfloat offset;
    switch (type) {
    case SECONDS:
        alGetSourcef(source_vec[0], AL_SEC_OFFSET, &offset);
        break;
    case SAMPLES:
        alGetSourcef(source_vec[0], AL_SAMPLE_OFFSET, &offset);
        break;
    }
    return offset;
}

bool Workspace::get_playing_state()
{
    return is_playing;
}

