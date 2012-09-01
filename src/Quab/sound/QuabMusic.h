#ifndef QUAB_MUSIC_H
#define QUAB_MUSIC_H

#include "../QuabDef.h"

namespace Quab
{
	interface IQuabMusic
	{
    public:
        virtual void play()   = 0;
        virtual void stop()   = 0;
        virtual void pause()  = 0;
        virtual void resume() = 0;
	};

	class QuabBGM:public IQuabMusic
	{
    private:
        const char *_soundStream;
    public:
        QuabBGM(const char *bgmFile);
        QuabBGM(const char *bgmStream, unsigned sz);
        ~QuabBGM();

        void play();
        void stop();
        void pause();
        void resume();
	};

	class QuabMusicEffect:public IQuabMusic
	{
    private:
        const char *_soundStream;
    public:
        QuabMusicEffect(const char *effectFile);
        QuabMusicEffect(const char *effectStream, unsigned sz);
        ~QuabMusicEffect();

        void play();
        void stop();
        void pause();
        void resume();
	};
}

#endif
