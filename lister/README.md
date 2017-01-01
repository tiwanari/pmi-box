lister
===

[counter](./counter) が出力した結果をもとに
Turney の極性指標 (SO 値) に基いての k 個の best / worst の

参考: [Turneyらの評価極性の分類]( https://goo.gl/1AuW9W )

# Usage
## run

```
./run.rb --input counted.csv -k k_items(default: 10)
e.g., ./run.rb --input counted.csv -k 100
```

#### 入力
[counter](./counter) が出力した結果です．
詳しくは [README](../counter/README.md) をご覧ください．

#### 出力
出力は標準出力に吐かれます．
