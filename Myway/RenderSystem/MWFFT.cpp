#include "MWFFT.h"

namespace Myway {
    inline float uniform_deviate()
    {
        return rand() * ( 1.0f / ( RAND_MAX + 1.0f ) );
    }

    FFT::FFT()
        : resolution(128)
        , re(0)
        , img(0)
        , maximalValue(2)
        , initialWaves(0)
        , currentWaves(0)
        , angularFrequencies(0)
        , time(10)
        , mInited(false)
    {
    }

    FFT::FFT(const Options &Options)
        : mOptions(Options)
        , resolution(128)
        , re(0)
        , img(0)
        , maximalValue(2)
        , initialWaves(0)
        , currentWaves(0)
        , angularFrequencies(0)
        , time(10)
        , mInited(false)
    {
    }

    FFT::~FFT()
    {
        remove();
    }

    void FFT::create()
    {
        if (isCreated())
        {
            return;
        }

        _initNoise();
    }

    void FFT::remove()
    {
        if (!isCreated())
        {
            return;
        }

        if (currentWaves)
        {
            delete [] currentWaves;
        }
        if (re)
        {
            delete [] re;
        }
        if (img)
        {
            delete [] img;
        }
        if (initialWaves)
        {
            delete [] initialWaves;
        }

        if (angularFrequencies)
        {
            delete [] angularFrequencies;
        }

        maximalValue = 2;
        time = 10;

        mInited = false;
    }

    void FFT::setOptions(const Options &Options)
    {
        if (isCreated())
        {
            if (mOptions.Resolution != Options.Resolution ||
                mOptions.Amplitude != Options.Amplitude ||
                mOptions.KwPower != Options.KwPower ||
                mOptions.PhysicalResolution != Options.PhysicalResolution ||
                mOptions.WindDirection != Options.WindDirection)
            {
                remove();

                mOptions = Options;
                resolution = Options.Resolution;

                create();

                return;
            }
            else
            {
            }
        }

        mOptions = Options;
        resolution = Options.Resolution;
    }

    void FFT::update(const float &timeSinceLastFrame)
    {
        _calculeNoise(timeSinceLastFrame);
    }

    void FFT::_initNoise()
    {
        initialWaves = new std::complex<float>[resolution*resolution];
        currentWaves = new std::complex<float>[resolution*resolution];
        angularFrequencies = new float[resolution*resolution];

        re  = new float[resolution*resolution];
        img = new float[resolution*resolution];

        Vec2 wave = Vec2(0,0);

        std::complex<float>* pInitialWavesData = initialWaves;
        float* pAngularFrequenciesData = angularFrequencies;

        float u, v,
            temp;

        for (u = 0; u < resolution; u++)
        {
            wave.x = (-0.5f * resolution + u) * (2.0f* Math::PI_1 / mOptions.PhysicalResolution);

            for (v = 0; v < resolution; v++)
            {
                wave.y = (-0.5f * resolution + v) * (2.0f* Math::PI_1 / mOptions.PhysicalResolution);

                temp = Math::Sqrt(0.5f * _getPhillipsSpectrum(wave, mOptions.WindDirection, mOptions.KwPower));
                *pInitialWavesData++ = std::complex<float>(_getGaussianRandomFloat() * temp, _getGaussianRandomFloat() * temp);

                temp=9.81f * wave.Length();
                *pAngularFrequenciesData++ = Math::Sqrt(temp);
            }
        }

        _calculeNoise(0);

        mInited = true;
    }

    void FFT::_calculeNoise(const float &delta)
    {
        time += delta*mOptions.AnimationSpeed;

        std::complex<float>* pData = currentWaves;

        int u, v;

        float wt,
            coswt, sinwt,
            realVal, imagVal;

        for (u = 0; u < resolution; u++)
        {
            for (v = 0; v< resolution ; v++)
            {
                const std::complex<float>& positive_h0 = initialWaves[u * (resolution)+v];
                const std::complex<float>& negative_h0 = initialWaves[(resolution-1 - u) * (resolution) + (resolution-1- v)];

                wt = angularFrequencies[u * (resolution) + v] * time;

                coswt = Math::Cos(wt);
                sinwt = Math::Sin(wt);

                realVal =
                    positive_h0.real() * coswt - positive_h0.imag() * sinwt + negative_h0.real() * coswt - (-negative_h0.imag()) * (-sinwt),
                    imagVal =
                    positive_h0.real() * sinwt + positive_h0.imag() * coswt + negative_h0.real() * (-sinwt) + (-negative_h0.imag()) * coswt;

                *pData++ = std::complex<float>(realVal, imagVal);
            }
        }

        _executeInverseFFT();
        _normalizeFFTData(0);
    }

    const float FFT::_getGaussianRandomFloat() const
    {
        float x1, x2, w, y1;

        do 
        {
            x1 = 2.0f * uniform_deviate() - 1.0f;
            x2 = 2.0f * uniform_deviate() - 1.0f;

            w = x1 * x1 + x2 * x2;

        } while ( w >= 1.0f );

        w = Math::Sqrt( (-2.0f * Math::Log( w ) ) / w );
        y1 = x1 * w;

        return y1;
    }

    const float FFT::_getPhillipsSpectrum(const Vec2& waveVector, const Vec2& wind, const float& kwPower_) const
    {
        // Compute the length of the vector
        float k = waveVector.Length();

        // To avoid division by 0
        if (k < 0.0000001f) 
        {
            return 0;
        }
        else
        {
            float windVelocity = wind.Length(),
                l = pow(windVelocity,2.0f)/9.81f,
                dot=waveVector.Dot(wind);

            return mOptions.Amplitude*
                (Math::Exp(-1 / pow(k * l,2)) / (Math::Pow(k,2) *
                 Math::Pow(k,2))) * Math::Pow(-dot/ (k * windVelocity), kwPower_);
        }
    }

    void FFT::_executeInverseFFT()
    {
        int l2n = 0, p = 1; 
        while (p < resolution) 
        {
            p *= 2; l2n++;
        }
        int l2m = 0; p = 1; 
        while (p < resolution) 
        {
            p *= 2; l2m++;
        }

        resolution = 1<<l2m; 
        resolution = 1<<l2n; 

        int x, y, i;

        for(x = 0; x <resolution; x++)
        {
            for(y = 0; y <resolution; y++) 
            {
                re[resolution * x + y] = currentWaves[resolution * x + y].real();
                img[resolution * x + y] = currentWaves[resolution * x + y].imag();
            } 
        }

        //Bit reversal of each row
        int j, k;
        for(y = 0; y < resolution; y++) //for each row
        {
            j = 0;
            for(i = 0; i < resolution - 1; i++)
            {
                re[resolution * i + y] = currentWaves[resolution * j + y].real();
                img[resolution * i + y] = currentWaves[resolution * j + y].imag();

                k = resolution / 2;
                while (k <= j) 
                {
                    j -= k; 
                    k/= 2;
                }

                j += k;
            }
        }

        //Bit reversal of each column
        float tx = 0, ty = 0; 
        for(x = 0; x < resolution; x++) //for each column
        {
            j = 0;
            for(i = 0; i < resolution - 1; i++)
            {
                if(i < j)
                {
                    tx = re[resolution * x + i];
                    ty = img[resolution * x + i];
                    re[resolution * x + i] = re[resolution * x + j];
                    img[resolution * x + i] = img[resolution * x + j];
                    re[resolution * x + j] = tx;
                    img[resolution * x + j] = ty;                      
                }  
                k = resolution / 2;
                while (k <= j) 
                {
                    j -= k; 
                    k/= 2;
                }
                j += k;
            }
        }       

        //Calculate the FFT of the columns
        float ca, sa,
            u1, u2,
            t1, t2,
            z;

        int l1, l2,
            l,  i1;

        for(x = 0; x < resolution; x++) //for each column
        {  
            //This is the 1D FFT:
            ca = -1.0;
            sa = 0.0;
            l1 = 1, l2 = 1;

            for(l=0;l<l2n;l++)
            {
                l1 = l2;
                l2 *= 2;
                u1 = 1.0;
                u2 = 0.0;
                for(j = 0; j < l1; j++)
                {
                    for(i = j; i < resolution; i += l2)
                    {
                        i1 = i + l1;
                        t1 = u1 * re[resolution * x + i1] - u2 * img[resolution * x + i1];
                        t2 = u1 * img[resolution * x + i1] + u2 * re[resolution * x + i1];
                        re[resolution * x + i1] = re[resolution * x + i] - t1;
                        img[resolution * x + i1] = img[resolution * x + i] - t2;
                        re[resolution * x + i] += t1;
                        img[resolution * x + i] += t2;
                    }
                    z =  u1 * ca - u2 * sa;
                    u2 = u1 * sa + u2 * ca;
                    u1 = z;
                }
                sa = Math::Sqrt((1.0f - ca) / 2.0f);
                ca = Math::Sqrt((1.0f+ca) / 2.0f);
            }
        }
        //Calculate the FFT of the rows
        for(y = 0; y < resolution; y++) //for each row
        {  
            //This is the 1D FFT:
            ca = -1.0;
            sa = 0.0;
            l1= 1, l2 = 1;

            for(l = 0; l < l2m; l++)
            {
                l1 = l2;
                l2 *= 2;
                u1 = 1.0;
                u2 = 0.0;
                for(j = 0; j < l1; j++)
                {
                    for(i = j; i < resolution; i += l2)
                    {
                        i1 = i + l1;
                        t1 = u1 * re[resolution * i1 + y] - u2 * img[resolution * i1 + y];
                        t2 = u1 * img[resolution * i1 + y] + u2 * re[resolution* i1 + y];
                        re[resolution * i1 + y] = re[resolution * i + y] - t1;
                        img[resolution * i1 + y] = img[resolution * i + y] - t2;
                        re[resolution * i + y] += t1;
                        img[resolution * i + y] += t2;
                    }
                    z =  u1 * ca - u2 * sa;
                    u2 = u1 * sa + u2 * ca;
                    u1 = z;
                }
                sa = Math::Sqrt((1.0f - ca) / 2.0f);
                ca = Math::Sqrt((1.0f+ca) / 2.0f);
            }
        }

        for(x=0;x<resolution;x++)
        {
            for(y=0;y<resolution;y++)
            {
                if (((x+y) & 0x1)==1)
                {
                    re[x*resolution+y]*=1;
                }
                else
                {
                    re[x*resolution+y]*=-1;
                }
            }
        }
    }

    void FFT::_normalizeFFTData(const float& scale)
    {
        float scaleCoef = 0.000001f;
        int i;

        // Perform automatic detection of maximum value
        if (scale == 0.0f)
        {
            float min=re[0], max=re[0],
                currentMax=maximalValue;;

            for(i=1;i<resolution*resolution;i++)
            {
                if (min>re[i]) min=re[i];
                if (max<re[i]) max=re[i];
            }

            min=Math::Abs(min);
            max=Math::Abs(max);

            currentMax = (min>max) ? min : max;

            if (currentMax>maximalValue) maximalValue=currentMax;

            scaleCoef += maximalValue;
        }
        else
        {	// User defined scale		
            scaleCoef=scale;
        }

        // Scale all the value, and clamp to [0,1] range
        int x, y;
        for(x=0;x<resolution;x++)
        {
            for(y=0;y<resolution;y++)
            {
                i=x*resolution+y;
                re[i]=(re[i]+scaleCoef)/(scaleCoef*2);			
            }
        }
    }

    float FFT::getValue(const float &x, const float &y)
    {
        // Scale world coords
        float xScale = x*mOptions.Scale,
            yScale = y*mOptions.Scale;

        // Convert coords from world-space to data-space
        int xs = static_cast<int>(xScale)%resolution,
            ys = static_cast<int>(yScale)%resolution;

        // If data-space coords are negative, transform it to positive
        if (x<0) xs += resolution-1;
        if (y<0) ys += resolution-1;

        // Determine x and y diff for linear interpolation
        int xINT = (x>0) ? static_cast<int>(xScale) : static_cast<int>(xScale-1),
            yINT = (y>0) ? static_cast<int>(yScale) : static_cast<int>(yScale-1);

        // Calculate interpolation coeficients
        float xDIFF  = xScale-xINT,
            yDIFF  = yScale-yINT,
            _xDIFF = 1-xDIFF,
            _yDIFF = 1-yDIFF;

        // To adjust the index if coords are out of range
        int xxs = (xs==resolution-1) ? -1 : xs,
            yys = (ys==resolution-1) ? -1 : ys;

        //   A      B
        //     
        //
        //   C      D
        float A = re[(ys*resolution+xs)],
            B = re[(ys*resolution+xxs+1)],
            C = re[((yys+1)*resolution+xs)],
            D = re[((yys+1)*resolution+xxs+1)];

        // Return the result of the linear interpolation
        return (A*_xDIFF*_yDIFF +
            B* xDIFF*_yDIFF +
            C*_xDIFF* yDIFF +
            D* xDIFF* yDIFF) // Range [-0.3, 0.3]
            *0.6f-0.3f;
    }
}