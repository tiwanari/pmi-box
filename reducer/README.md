reducer
===

[counter](./counter) が出力した結果を複数受け取ってまとめるプログラムです．

counter において，
同じ形容詞・対義語・品詞を指定して処理した結果のファイルを複数受け取り，
各回数を合計します．
また，reducer 自身が出力したファイルをさらに reducer へ入力することも可能です．

**NOTE: 語彙数については，現状対応できていません．**

これによって，並列で処理した counter の結果をまとめあげて，
[lister](../lister) に渡すことができます．

# Usage
## make

_reducer_ フォルダにて `make` でコンパイルされます．`make test` でテストが走ります (テストコードは不十分です…)．

## run

```
usage: ./main --input_list_file=string --output_file=string [options] ...
options:
  -i, --input_list_file    a file shows a list of inputs (string)
  -o, --output_file        a file for output (string)
  -?, --help               print this message./main input_list_file output_file

e.g., ./main -i counted_cvs.txt -o output.csv
```

`input_list_file` は，1 行に 1 ファイルを指定した
以下のようなファイルになります．

```
./reducer/unittest/dataset/reducer_test_data0.txt
./reducer/unittest/dataset/reducer_test_data1.txt
```

### 入力
`input_list_file` の各行が表すファイルは次のようなフォーマットです．

```
 1| ADJ, adjective
 2| ANT, antonym
 3| TAG, tag
 4| TOTAL_WORDS, total_words
 5| VOCABULARY, -1
 6| POS_OCCURRENCES, pos_occurrences
 7| NEG_OCCURRENCES, neg_occurrences
 8| OC, target0, 100
 9| P_COOC, target0, 10
10| N_COOC, target0, 10
11| OC, target1, 200
12| ...
```

**NOTE: 8 行目以降はどのような順で現れても構いません．**


#### 出力

出力は以下のようなフォーマットです．
8 行目以降はテキストに含まれる対象品詞の数だけ繰り返しとなり，
また順序は元のファイルから保存されません
(counter は `OC`・`P_COOC`・`N_COOC` を単語ごとに順番に出力しますが，
reducer は `N_COOC` をまとめて出力します)．

```
 1| ADJ, adjective
 2| ANT, antonym
 3| TAG, tag
 4| TOTAL_WORDS, total_words
 5| VOCABULARY, -1
 6| POS_OCCURRENCES, pos_occurrences
 7| NEG_OCCURRENCES, neg_occurrences
 8| N_COOC, target0, 10
 9| N_COOC, target1, 10
12| ...
```
