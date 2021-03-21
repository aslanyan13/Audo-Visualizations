#include <SFML/Graphics.hpp>

#include <iostream>
#include <time.h>
#include <math.h>
#include <windows.h>

#include "bass.h"

using namespace std;
using namespace sf;

int main()
{
    srand(time(NULL));

    RenderWindow window (VideoMode(400, 400), "Circle spectrum");
    window.setFramerateLimit(120);   // FPS limit to 60

    const int pointsN = 720;

    HSTREAM chan = NULL;
    float fft[1024];

    HWND win = NULL;
    wchar_t file[MAX_PATH] = L"";

    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = win;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFile = file;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;
    ofn.lpstrTitle = L"Select a file to play";
    ofn.lpstrFilter = L"playable files\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";

    if (!GetOpenFileName(&ofn)) return FALSE;

    BASS_Init(-1, 44100, BASS_DEVICE_16BITS | BASS_DEVICE_STEREO, 0, NULL);
    BASS_SetConfig(BASS_CONFIG_SRC, 16);

    cout << (chan = BASS_StreamCreateFile(FALSE, file, 0, 0, 0)) << endl;
    BASS_ChannelPlay(chan, FALSE);

    while (window.isOpen())
    {
        window.clear();

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        BASS_ChannelGetData(chan, fft, BASS_DATA_FFT2048);

        for (float i = 0; i <= pointsN / 2; i++)
        {
            float h = sqrt(fft[(int)i + 1]) * 3 * 100 - 4;
            if (h > 100) h = 100;
            if (h < 0) h = 0;

            h = 1.02f + (h / 100);

            VertexArray lines(LinesStrip, 2);

            float angle = i/pointsN * 6.28 - 3.14159 / 2;

            lines[0] = Vector2f (200 + 100 * cos (angle), 200 + 100 * sin (angle));
            lines[1] = Vector2f (200 + 100 * h * cos (angle), 200 + 100 * h * sin (angle));

            window.draw(lines);
        }

        for (float i = 0; i <= pointsN / 2; i++)
        {
            float h = sqrt(fft[(int)i + 1]) * 3 * 100 - 4;
            if (h > 100) h = 100;
            if (h < 0) h = 0;

            h = 1.02f + (h / 100);

            VertexArray lines(LinesStrip, 2);
            float angle = i/pointsN * 6.28 - 3.14159 / 2;

            lines[0] = Vector2f (200 - 100 * cos (angle), 200 + 100 * sin (angle));
            lines[1] = Vector2f (200 - 100 * h * cos (angle), 200 + 100 * h * sin (angle));

            window.draw(lines);
        }

        CircleShape circ(100, 60);
        circ.setOrigin(100, 100);
        circ.setPosition(200, 200);
        circ.setFillColor(Color::Transparent);
        window.draw(circ);

        window.display();
    }

    return 0;
}
