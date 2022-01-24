This file is just for me, to remember what happened because because I'm writing a thesis about it.




#+-------------------------------------------- 24.02.2022 --------------------------------------------+

Das Einführen scale in den Textures löste den Bug mit den weissen Linien.
-> Engine kann jetzt endlich sogar für Rendering benutzt werden :D.

Performance Verbesserungen durch die Verwendung von "memcpy" beim clearen von layers.

Ich habe eine kleine scene kreiert um pixa zu testen:
<p align="center">
```c

void main_onUpdate()
{
    clear_layer();

    for (int y = 0; y < get_width(); y += 10)
        for(int x = 0; x < get_height(); x += 10)
            draw_pixel(x, y);
}

int main()
{
    create_engine(500, 500, 1, 1);
    create_scene(NULL, main_onUpdate, NULL);

    set_clear_color(COLOR_VERY_DARK_GREY);

    start_engine();
}
```
</p>

![engine_test](./img/engine_test_24.02.2022.png)

Die scene rendert mit bis zu 3500 FPS.
