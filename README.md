# Luxo jr.

![Luxo](/images/LuxoV2.jpeg)

Luxo jr. je jen hříčka – robotická lampa inspirovaná Luxem jr. od firmy Pixar.

### Historie

Na rozdíl od ostatních projektů nebyla lampa nikdy zamýšlena jako seriózní projekt. Cílem bylo vytvořit „zlobivou lampu“, která jen vyrušuje. Součástí tohoto „řešení“ měla být robotická lampa.

To se nakonec podařilo hned dvakrát – vznikly prototypy V1 a V2, které se od sebe liší snad úplně ve všem.

Druhou částí řešení je modul computer vision, který má zajišťovat „oči“ lampy. Jeho realizace zatím čeká.

### HW

Obě lampy jsou kompletně uzavřené webové systémy, které obsahují veškerou elektroniku potřebnou k pohybu, webový server a REST API.

Lampy jsou dostupné na adresách `10.0.0.95` (V1) a `10.0.0.84` (V2). Pohání je procesor ESP32 a ovládání probíhá přes REST API.

#### V1

První verze je postavena na klasických RC servách. Je plně funkční, jen některá serva už jsou tzv. na hraně.

Funkcionalitu lampy je možné vidět na [tomto videu](https://photos.app.goo.gl/dxM9xuKke8i4iCDH8). Na stejném principu jsou založeny také robotické pohony LX-16, které jsou použity ve verzi V2.

![Luxo](/images/LuxoV1.jpeg)
![Luxo](/images/LuxoV1ctl.jpeg)

#### V2

RC serva byla nahrazena pohony Lobot LX-16. Jednalo se o natolik zásadní změnu, že se vyplatilo postavit kompletně nový prototyp.

![Luxo](/images/LuxoV2.jpeg)
![Luxo](/images/LuxoV2ctl.jpeg)

Obě lampy najednou jsou vidět [zde](https://photos.app.goo.gl/JdUXHv19hhEEWmWk9).

### Stav projektu

Vše je plně funkční a samostatný vývoj lamp je ukončen. Lampy jsou připojeny k domácí Wi-Fi síti a jsou řízeny centrálním počítačem.

V sekci `modely` jsou k dispozici kompletní data pro tisk převodovek pro verzi V1 i V2.

Elektroniku tvoří regulovaný spínaný zdroj v podstavci lampy spolu s ESP32. To přímo ovládá serva u verze V1, případně interface desku pro pohony LX-16 u verze V2. Pohony LX-16 jsou zapojeny v režimu daisy chain.

Světelný zdroj ovládá malý relemodul.
