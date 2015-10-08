#ifndef DEFINES_H
#define DEFINES_H

#endif // DEFINES_H

typedef struct st_angle_item
{
    double baseAngle;
    double neckAngle;
    int cropX;
    int cropY;
    int cropWidth;
    int cropWeight;
    st_angle_item()
    {
        baseAngle = 0.0;
        neckAngle = 0.0;
        cropX = 0;
        cropY = 0;
        cropWidth = 0;
        cropWeight = 0;
    }
    st_angle_item(double _baseAngle, double _neckAngle, int _cropX, int _cropY, int _cropWidth, int _cropWeight) :
        baseAngle(_baseAngle), neckAngle(_neckAngle), cropX(_cropX), cropY(_cropY), cropWidth(_cropWidth), cropWeight(_cropWeight)
    {

    }
}AngleItem;

