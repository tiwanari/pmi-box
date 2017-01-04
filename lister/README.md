lister
===

[counter](./counter) が出力した結果をもとに
Turney の極性指標 (SO 値) に基いての k 個の best / worst の
単語を出力するプログラムです．

参考: [Turneyらの評価極性の分類]( https://goo.gl/1AuW9W )

# Usage
## run

```
./run.rb --input counted.csv -k (default: 10) --min-occurrence (default: 1)
e.g., ./run.rb --input counted.csv -k 100
```

なお， `--input` や `--min-occurrence` には， `-i` や `-m` が代わりに使えます．

#### 入力
[counter](./counter) が出力した結果です．
詳しくは [README](../counter/README.md) をご覧ください．

#### 出力
出力は標準出力に吐かれます．

e.g.,

```sh
$ ./run.rb -i test_100000000.csv -k 10 -m 5
input file: test_100000000.csv
k: 10
min occurrence: 5
reading test_100000000.csv...
read!
calculating so-scores...
calculated!

===== output =====
adjective: 大きい <-> antonym: 小さい
tag: ADJECTIVE
total_words: 75312446, vocabulary: 1053725
pos_occurrences: 16960, neg_occurrences: 4444

ignored: 2362
best  K:
[["好調だ", 1.7519057314344817],
 ["大幅だ", 1.3908923858971507],
 ["珍しくない", 1.3689134791783761],
 ["微妙だ", 1.2999206076914245],
 ["偉大だ", 1.225812635537702],
 ["世界的だ", 1.225812635537702],
 ["立派だ", 1.225812635537702],
 ["余儀ない", 1.1998371491344413],
 ["活発だ", 1.1865919223844212],
 ["同等だ", 1.1457699278641655]]
worst K:
[["幸せだ", -2.948574634357935],
 ["大きいない", -2.843214118700109],
 ["大きくない", -2.550226994018634],
 ["生意気だ", -2.4377490105919444],
 ["うれしい", -2.4377490105919444],
 ["勇敢だ", -2.4377490105919444],
 ["理不尽だ", -2.4377490105919444],
 ["広くない", -2.4377490105919444],
 ["華美だ", -2.4377490105919444],
 ["善良だ", -2.4377490105919444]]
```
