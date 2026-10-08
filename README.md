# Luxo jr.

![Luxo](/images/LuxoV2.jpeg)

Luxo jr. je jen hricka - roboticka lampa inspirovana Luxi jr. od firmy Pixar

### Historie:

Na rozdil od ostatnich, nebyla lampa niky minena jako seriozni projekt. Cilem hry
bylo mit "zlobivou lampu", ktera jen vyrusuje. Soucasti "reseni" mela byt roboticka
lampa. Coz se stalo hned dvakrat - existuji prototypy V1 a V2 ktere se lisi snad uplne ve vsem.
Druha cast reseni je computer vision modul ktery zajistuje oci lampy. Ten ne realizaci teprve ceka

### HW

Obe lampy jsou kompletne uzavrene webove systemy, ktere obsahuji veskerou elektroniku potrebnou k pohybu
+ web server + rest-api. Lampy se hlasi na 10.0.0.95 (V1) a 10.0.0.84 (V2). Pohani je processor ESP32
a pres rest-api 

#### V1
Prvni verze je postavena na klasickych RC servech. je plne funkcni, jen nektera serva uz jsou tzv. na hrane.
Funkcionalita je videt na videu https://photos.app.goo.gl/dxM9xuKke8i4iCDH8 ..a stejne tak roboticke pohony
LX-16 ktere jsou zakladem verze V2


![Luxo](/images/LuxoV1.jpeg)
![Luxo](/images/LuxoV1ctl.jpeg)

#### V2
RC serva jsou nahrazena pohony Lobot LX-16. Jednalo se o tak velkou zmenu, ze se vyplatilo postavi komplet druhy prototyp.
![Luxo](/images/LuxoV2.jpeg)
![Luxo](/images/LuxoV2ctl.jpeg)

Obe lampy najednou jsou pak videt zde: https://photos.app.goo.gl/JdUXHv19hhEEWmWk9

Vse je plne funkcni, samostatny vyvoj lamp je ukoncen. Lampy jsou pripojeny na domaci wifi sit a jsou rizeny
centralnim pocitacem. V sekci modely jsou kompletni data na tisk prevodovek jak pro verzi V1 tak i V2.
Elektroniku tvori regulovany spinany zdroj v podstavci lamp spolu s ESP32 ktere primo ovlada serva, nebo
interface desku na LX-16 ktera jedou v rezimu daisy chain. Svetelny zdroj ovlada maly servomodul.

