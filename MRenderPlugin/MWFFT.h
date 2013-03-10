#pragma once

#include <complex>
#include "MWMath.h"

namespace Myway {

    class FFT
	{
        DECLARE_ALLOC();

	public:
		/** Struct wich contains fft noise module options
		 */
		struct Options
		{
			/// Noise resolution (2^n)
			int Resolution;
			/// Physical resolution
			float PhysicalResolution;
			/// Noise scale
			float Scale;
			/// Wind direction
			Vec2 WindDirection;
			/// Animation speed
			float AnimationSpeed;
			/// KwPower
			float KwPower;
			/// Noise amplitude
			float Amplitude;


			/** Default constructor
			 */
			Options()
				: Resolution(128)
				, PhysicalResolution(32.0f)
			    , Scale(0.25f)
				, WindDirection(Vec2(4,5))
				, AnimationSpeed(1)
				, KwPower(6.0f)
				, Amplitude(1.0f)
			{
			}

			/** User constructor
			    @param _Resolution FFT Resolution (2^n)
				@param _PhysicalResolution Physical resolution of the surface
			    @param _Scale Noise scale
				@param _WindDirection Wind direction
				@param _AnimationSpeed Animation speed coeficient
				@param _KwPower KwPower
				@param _Amplitude Noise amplitude
			 */
			Options(const int&           _Resolution,
				    const float&         _PhysicalResolution,
				    const float&         _Scale,
					const Vec2& _WindDirection,
					const float&         _AnimationSpeed,
					const float&         _KwPower,
					const float&         _Amplitude)
				: Resolution(_Resolution)
				, PhysicalResolution(_PhysicalResolution)
			    , Scale(_Scale)
				, WindDirection(_WindDirection)
				, AnimationSpeed(_AnimationSpeed)
				, KwPower(_KwPower)
				, Amplitude(_Amplitude)
			{
			}

			/** User constructor
			    @param _Resolution FFT Resolution (2^n)
				@param _PhysicalResolution Physical resolution of the surface
			    @param _Scale Noise scale
				@param _WindDirection Wind direction
				@param _AnimationSpeed Animation speed coeficient
				@param _KwPower KwPower
				@param _Amplitude Noise amplitude
			 */
			Options(const int&           _Resolution,
				    const float&         _PhysicalResolution,
				    const float&         _Scale,
					const Vec2& _WindDirection,
					const float&         _AnimationSpeed,
					const float&         _KwPower,
					const float&         _Amplitude,
					const float &_GPU_Strength,
					const Vec3 &_GPU_LODParameters)
				: Resolution(_Resolution)
				, PhysicalResolution(_PhysicalResolution)
			    , Scale(_Scale)
				, WindDirection(_WindDirection)
				, AnimationSpeed(_AnimationSpeed)
				, KwPower(_KwPower)
				, Amplitude(_Amplitude)
			{
			}
		};

		/** Default constructor
		 */
		FFT();

		/** Constructor
		    @param Options FFT noise options
		 */
		FFT(const Options &Options);

		/** Destructor
		 */
		~FFT();

		/** Call it each frame
		    @param timeSinceLastFrame Time since last frame(delta)
		 */
		void update(const float &timeSinceLastFrame);


		/** Get the especified x/y noise value
		    @param x X Coord
			@param y Y Coord
			@return Noise value
		 */
		float getValue(const float &x, const float &y);

		/** Set/Update fft noise options
		    @param Options FFT noise options
		 */
		void setOptions(const Options &Options);

		/** Get current FFT noise options
		    @return Current fft noise options
		 */
		inline const Options& getOptions() const
		{
			return mOptions;
		}

	private:
        /** Create
		 */
		void create();

		/** Remove
		 */
		void remove();

        bool isCreated() { return mInited; }

		/** Initialize noise
		 */
		void _initNoise();

		/** Calcule noise
		    @param delta Time elapsed since last frame
		 */
		void _calculeNoise(const float &delta);

		/** Execute inverse fast fourier transform
		 */
		void _executeInverseFFT();

		/** Normalize fft data
		    @param scale User defined scale
		 */
		void _normalizeFFTData(const float& scale);

		/** Get the Philipps Spectrum, used to create the amplitudes and phases
		    @param waveVector Wave vector
			@param wind Wind direction
			@param kwPower_ kwPower
			@return Philipps Spectrum
		 */
		const float _getPhillipsSpectrum(const Vec2& waveVector, const Vec2& wind, const float& kwPower_ = 2.0f) const;

	    /** Get a Gaussian random number with mean 0 and standard deviation 1, using Box - muller transform
		    @return Gaussian random number with mean 0 and standard deviation 1, using Box - muller transform
		 */
	    const float _getGaussianRandomFloat() const;

		/// FFT resolution
		int resolution;
		/// Pointers to resolution*resolution float size arrays
    	float *re, *img;
	    /// The minimal value of the result data of the fft transformation
    	float maximalValue;

		/// the data which is referred as h0{x,t), that is, the data of the simulation at the time 0.
	    std::complex<float> *initialWaves;
	    /// the data of the simulation at time t, which is formed using the data at time 0 and the angular frequencies at time t
	    std::complex<float> *currentWaves;
	    /// the angular frequencies
	    float  *angularFrequencies;
		/// Current time
		float time;

		/// Perlin noise options
		Options mOptions;

        bool mInited;
	};
}