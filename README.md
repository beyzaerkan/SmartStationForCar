# Smart Station For Car

## Senaryo 
Kısaca IoT tabanlı bir araç kiralama uygulamasıdır. Kullanıcı aracın istasyonda olup olmadığını 
sistemden görüntüleyebilir. Eğer araç istasyonda ise uygulama üzerinden kiralama butonuna 
tıklar. Butona tıklandığında kullanıcıdan NFC – RFID kartını okutması istenir. Kart okunduktan 
sonra kullanıcının sisteme kayıtlı olup olmadığı kontrol edilir. Eğer sisteme kayıtlı ise araç 
kiralanır. Değilse kiralayamaz, erişim engellendi uyarısı verilir. Kiralanan araç iade edilebilir.

## Kullanılan Teknolojiler
- NodeMCU 
- RC522 RFID Modülü  
- HC-SR04 Ultrasonik Uzaklık Sensörü 
- RGB led 
- Jumper kablolar  
- Blynk 
- ThingSpeak

## Devre Şeması
![Devre şeması](/devre-sema.png)

## Büyük Veri
İstasyonların kullanım sıklığı kontrol edilerek insanların kendine ait ulaşım aracına ihtiyacı 
azalıp artabilir. 
Uygulama geliştirilerek aynı güzergaha gidecek kullanıcıların aynı aracı kullanması 
sağlanabilir.  
Araçlara sensörler eklenerek kazaya yol açan nedenler bulunabilir. Hangi konumda ne kadar 
hız yapıldığı kontrol edilebilir. Kaza oranları tespit edilir. 
NFC – RFID kart kullanmak yerine direkt uygulama üzerinden kiralama yapabilme 
sağlanabilir.  
Bu veriler doğrultusunda insanların şahsi araca ihtiyacını azaltmak için istasyonlar 
çoğaltılabilir. Kullanıcılar kendi araçlarını kullanmak yerine bu araçları tercih ederek enerji 
tasarrufu sağlayabilir.
