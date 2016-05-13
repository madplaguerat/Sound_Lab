#include "audio.h"
#include <string>
#include <vector>
#include <iostream>
#include <QDebug>

using namespace Audio;

Audiofile::Audiofile()
{
}

Audiofile::~Audiofile()
{
}

void Audiofile::getName(std::string path)
{
    std::size_t found_pos = path.find_last_of("/\\") + 1;
    if (found_pos <= path.length())
        this->file_name = path.substr(found_pos);
    else
        this->file_name = path;
}

bool Audiofile::loadData(std::string path)
{
    //Convert absolute path into ALbyte format
    ALbyte *file_name = new ALbyte[path.size() + 1];
    std::copy(path.begin(), path.end(), file_name);
    file_name[path.size()] = '\0';

    //Load data from file
    ALvoid *data;
    ALboolean loop;
    alutLoadWAVFile(file_name, &(this->fdata.format), &data, &(this->fdata.size),
                    &(this->fdata.frequency), &loop);
    const char *auxPath = path.c_str();

    short numChannels;

    FILE *fp;
    fp = fopen(auxPath, "r");
    fseek(fp, 22, SEEK_SET);
    fread(&numChannels, sizeof(short), 1, fp);

    fseek(fp, 34, SEEK_SET);
    fread(&(this->fdata.bit_depth), sizeof(short), 1, fp);
    fclose(fp);

    //Copy data to left and right channels
    int *intData = reinterpret_cast<int *>(data);
    int len = this->fdata.size / sizeof intData[0];
    this->fdata.data_left = new int [len];
    this->fdata.data_right = new int [len];
    if(numChannels == 1)
    {
        for(int i = 0; i < len; ++i)
        {
            this->fdata.data_left[i] = intData[i];
            this->fdata.data_right[i] = intData[i];
        }
    }
    if(numChannels == 2)
    {
        for(int i = 0; i < len; ++i)
        {
            if (i % 2 == 0)
            {
                this->fdata.data_left[i] = intData[i];
                this->fdata.data_right[i] = 0;
            }
            else
            {
                this->fdata.data_right[i] = intData[i];
                this->fdata.data_left[i] = 0;
            }
        }
    }

}

bool Audiofile::loadData(struct file_inf input)
{
    int len = input.size * sizeof(int);
    this->fdata.data_left = new int[len];
    this->fdata.data_right = new int[len];
    this->fdata = input;
}

struct file_inf Audiofile::getData()
{
    return this->fdata;
}

int Audiofile::getAudioLength()
{
    return this->fdata.size / sizeof this->fdata.data_left[0];
}
