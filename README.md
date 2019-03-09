M5Stack Lovyan ToyBox
===

M5Stack用 らびやんの実験的な何か

## Description
  
M5Stackで作った諸々の詰め合わせです。  
  
* ScreenShotReceiver PCの画面をM5Stack上に表示  
* ScrollDemo   ili9341のスクロール機能を使ったミニゲーム  
* MPU9250Demo  9軸センサの動作デモ（GRAY/FIRE用）  
* ADInputDemo  アナログポートAD35/36の動作確認  
* FIRE MicFFT  FIREのマイク入力をFFT表示  
* IP5306 Registry 電源制御チップのI2Cレジストリの表示と編集  
  
※ OTA RollbackはSD-Updaterを使用している場合に別パテーションのアプリを起動します。  
  
実際の動きを以下のツイートの動画で見る事が出来ます。  
[機能紹介](https://mobile.twitter.com/lovyan03/status/1104025644202057729)  
[ScreenShotSender/Receiver](https://mobile.twitter.com/lovyan03/status/1104169374888255488)  
  
## Usage
 `BtnA click` : フォーカスを親へ移動  Move focus to parent item.  
 `BtnA hold`  : フォーカスを前へ移動  Move focus to the previous item.  
 `BtnB click` : 選択  Select focus item.  
 `BtnC click/hold` : フォーカスを次へ移動  Move focus to the next item.  
  
FACES/JoyStick/PLUSEncoder/CardKB にも対応しています。  
  
## Requirement
動作には以下のライブラリが必要です。  

* https://github.com/kosme/arduinoFFT/  
* https://github.com/tobozo/M5Stack-SD-Updater/  
* https://github.com/lovyan03/M5Stack_TreeView/  
* https://github.com/lovyan03/M5Stack_OnScreenKeyboard/  


## Licence

[LGPL v2.1](https://github.com/lovyan03/M5Stack_LovyanToyBox/blob/master/LICENSE)  

## Author

[lovyan03](https://twitter.com/lovyan03)  
