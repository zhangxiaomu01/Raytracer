#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H
#include "Texture.h"
#include "rtw_image.h"

class ImageTexture : public Texture
{
public:
    ImageTexture(const char* filename) : m_image(filename) {}

    Color value(double u, double v, const Point3& p) const override {
        if (m_image.height() <= 0) {
            return Color(0,1,1); // Debug color
        }

        u = Interval(0.0, 1.0).Clamp(u);
        v = 1.0 - Interval(0.0, 1.0).Clamp(v); // Flip v coordinate to match Image space

        auto i = int(u * m_image.width());
        auto j = int(v * m_image.height());
        auto pixel = m_image.pixel_data(i,j);

        auto color_scale = 1.0 / 255.0;
        return Color(pixel[0] * color_scale, pixel[1] * color_scale, pixel[2] * color_scale);
    }
private:
    rtw_image m_image;
};


#endif