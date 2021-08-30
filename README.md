# eventflg_test
Linux(及びアンドロイド)で、Win32のEvent的な動作をテストしたものです。  
当初eventfd + poolで実装していたのですが、あまりにもCPU負荷がスピンロック的であったため、kernelが待つというepollを利用しました。  

このソースはそのテスト用です。  
比較対象として、spinlock(atomic or volatile)で処理を行った場合をおいてあります。  
epool以外ではCPU負荷が高く、epoll, poolは処理に時間がかかります。  

求めていたのはepoolでした。  

結果はそのうち貼ります。  

# テスト環境
Ryzen7 5800X  
Windows10 Pro 21H1  
WSL2 + Ubuntu LTS20.04  

# Result
ループ回数=100000, 時間単位=msec  
|方法 | 時間 | 実行された回数 |
| ---- | ---- | ---- |
|epoll|1814.08||100001|  
|poll|1923.65|488037|  
|volatile spin|42.70|530635|  
|atomic spin|61.67|54435|  

epollだけ通った個数が違うのがちょっと面白かったです。  
この方が目的にも一致していた。  
