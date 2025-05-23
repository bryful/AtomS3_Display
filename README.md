# AtomS3_Display

M5Stack AtomS3に外部ディスプレイを接続してみるテスト<br>
<br>
つなげたディスプレイは"[WINGONEER 1.8インチフルカラー128×160 SPIフルカラーTFT LCDディスプレイモジュールST7735S 3.3V DIYキット用OLED電源の交換](https://www.amazon.co.jp/dp/B07QGCWJMV?ref=ppx_yo2ov_dt_b_fed_asin_title)"<Br>
<br>
[M5Stack AtomS3で外部ディスプレイ(ST7789)を使う](https://qiita.com/nobrin/items/c9490a2d1e16e849c5c2)を参考にしました。

C#製のWindowsアプリ[ComM5Atom](https://github.com/bryful/ComM5Atom)とシリアル通信のテストも行っています。


# PIN配置

* GND -> GND
* VCC -> 3.3V
* SCL -> G5
* SDA -> G6
* RES -> G7
* DC  -> G8
* CS  -> G38
* BLK -> G39

参考にはCSピンがなかったので適当なPINに割り振りました。

# LiiteFS
LittleFSに顔パーツの画像をあらかじめuploadが必要です。<br>
platformio.iniに設定した後、ターミナルで以下のコマンドでできます。

```
pio run -t uploadfs
```
ターミナルはPlatformIOで"New Terminal"した後プロジェクトファイルのあるフォルダへカレントを移動してからです。

## Dependency
VSCode PlatformIO

## License
This software is released under the MIT License, see LICENSE

## Authors

bry-ful(Hiroshi Furuhashi)<br>
twitter:[bryful](https://twitter.com/bryful)<br>
Mail: bryful@gmail.com<br>

