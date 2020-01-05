# Arduino EmotionBox
Emotion Box is a reminder for drinking water.

### Preparation
- Arduino UNO board *1
- HX711 weight-scale *1
- buzzer *1
- 8x8 matrix LED *1

### Wire Map
![Emotion_Cup_bb](https://user-images.githubusercontent.com/29245400/67158938-0df95500-f32e-11e9-9ce9-4d48b95f1653.jpg)


## [Arduino] Emotion Cup＿提醒喝水裝置

#### 運作步驟
1. 水杯放到秤上，8x8 矩陣 LED 顯示「笑臉」
2. 過 10 秒後，若測量到的重量沒有大幅減少（使用者未喝水），8x8 矩陣 LED 改為顯示「哭臉」，蜂鳴器短響一次。
3. 使用者喝水後（測量到的重量有大幅減少），8x8 矩陣 LED 從「哭臉」改為「笑臉」。

以上是一個循環。

> **_已完成的程式碼 👉[emotion_cup](https://gist.github.com/laiyenju/5c44d5153dbb176a4f5e3de4e61c9672#file-emotion_cup)_**
- 確定能比較重量，並依照重量改變而更換 LED 表情
**- 待解決：蜂鳴器無法發出聲音（但使用 `Serial.println("BUZZER!")` 檢查會回傳訊息）**

#### 使用的裝置
- 主機板：Arduino UNO
- HX711 秤重器
- 蜂鳴器
- 8x8 矩陣 LED 

#### 接線圖
> 有更新接線，因為 buzzer 的＋線在 Pin1 時，在 8x8 LED 發亮時，會同時發出叫聲。
> 因此將 buzzer ＋ 線改接到 Pin0，能成功運作，但卻必須要在上傳程式碼的時候，先拔掉插在 Pin0 的線，才能成功上傳。
