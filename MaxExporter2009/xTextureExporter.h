#pragma once


namespace MaxPlugin {

	class xTextureExporter
	{
		DECLARE_SINGLETON(xTextureExporter);

	public:
		xTextureExporter();
		~xTextureExporter();

		void Clear();

		void Push(const char * texture);

		void Export();

	protected:
		Map<TString128, int> mTextures;
	};
}