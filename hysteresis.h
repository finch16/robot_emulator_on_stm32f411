#ifndef H_HYSTERESIS_H
#define H_HYSTERESIS_H

int hysteresis_loop(const int h, int old_data, int new_data)
{
    static int dir = 0;
    if(dir)
    {
        if(old_data > new_data) return new_data; else if(new_data - old_data > h) { dir = 0; return new_data; } else return old_data;
    }
    else
    {
        if(new_data > old_data) return new_data; else if(old_data - new_data > h) { dir = 1; return new_data; } else return old_data;
    }
}

#endif //H_HYSTERESIS_H
