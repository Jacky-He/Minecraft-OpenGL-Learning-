#include "Noise.hpp"

Noise::Noise()
{
    //generate a random permutation from 0 to 255
    for (int i = 0; i < 256; i++) m_Permutation[i] = i;
    for (int i = 0; i < 256; i++) std::swap (m_Permutation[i], m_Permutation[Util::RandInt(i, 255)]);
    for (int i = 0; i < 512; i++) m_P[i] = m_Permutation[i%256];
}

Noise::~Noise()
{
    
}

double Noise::Lerp(double t, double a, double b)
{
    return a + t*(b - a);
}

double Noise::Grad(int hash, double x, double y, double z)
{
    //choose from the 12 directional vectors and dot them 
    switch (hash & 0xF)
    {
        case 0x0: return x + y;
        case 0x1: return -x + y;
        case 0x2: return x - y;
        case 0x3: return -x - y;
        case 0x4: return x + z;
        case 0x5: return -x + z;
        case 0x6: return x - z;
        case 0x7: return -x - z;
        case 0x8: return y + z;
        case 0x9: return -y + z;
        case 0xA: return y - z;
        case 0xB: return -y - z;
        case 0xC: return y + x;
        case 0xD: return -y + z;
        case 0xE: return y - x;
        case 0xF: return -y - z;
        default: return 0;
    }
    return 0;
}

double Noise::Fade(double t) //uses the fade curve: 6t^5 - 15t^4 + 10t^3
{
    return t*t*t*(t*(t*6-15)+10);
}

double Noise::GenNoise(double x, double y, double z)
{
    int unitx = int(floor(x))&255;
    int unity = int(floor(y))&255;
    int unitz = int(floor(z))&255;
    
    double xdis = x - int(floor(x));
    double ydis = y - int(floor(y));
    double zdis = z - int(floor(z));
    
    double u = Fade (xdis);
    double v = Fade (ydis);
    double w = Fade (zdis);
    
    //hash for 8 corners
    int aaa = m_P[m_P[m_P[unitx    ] + unity    ] + unitz    ];
    int aba = m_P[m_P[m_P[unitx    ] + unity + 1] + unitz    ];
    int aab = m_P[m_P[m_P[unitx    ] + unity    ] + unitz + 1];
    int abb = m_P[m_P[m_P[unitx    ] + unity + 1] + unitz + 1];
    int baa = m_P[m_P[m_P[unitx + 1] + unity    ] + unitz    ];
    int bba = m_P[m_P[m_P[unitx + 1] + unity + 1] + unitz    ];
    int bab = m_P[m_P[m_P[unitx + 1] + unity    ] + unitz + 1];
    int bbb = m_P[m_P[m_P[unitx + 1] + unity + 1] + unitz + 1];
    
    //output range [-2, 2];
    double output = Lerp (w, Lerp (v, Lerp (u, Grad(aaa, xdis, ydis, zdis),
                                               Grad(baa, xdis - 1, ydis, zdis)),
                                      Lerp (u, Grad(aba, xdis, ydis - 1, zdis),
                                               Grad(bba, xdis - 1, ydis - 1, zdis))),
                             Lerp (v, Lerp (u, Grad(aab, xdis, ydis, zdis - 1),
                                               Grad(bab, xdis - 1, ydis, zdis - 1)),
                                      Lerp (u, Grad(abb, xdis, ydis - 1, zdis - 1),
                                               Grad(bbb, xdis - 1, ydis - 1, zdis - 1))));
    //output range [0, 1];
    double normalized = (output + 2)/4.0;
    return normalized;
}
