counter
===

**極性を考慮して**，与えられた形容詞 (とその対義語) と，
対象とする品詞に属する単語との共起回数を数え上げるプログラムです．

「極性を考慮して」というのは，
形容詞 (例: 大きい) に否定を表す語「ない」が付属した場合 (例: 大きくない) に，
対義語 (例: 小さい) が現れたとすることです．
また，対義語に「ない」が付属した場合は，
(与えられた) 形容詞が現れたとします．

全単語数や語彙数も出力し，PMI への計算に利用します．

# Usage
## make

_counter_ フォルダにて `make` でコンパイルされます．`make test` でテストが走ります (テストコードは不十分です…)．

## run

```
usage: ./main --adjective=string --antonym=string --target_pos=string [options] ...
options:
  -a, --adjective     adjective (string)
  -n, --antonym       antonym (string)
  -p, --target_pos    POS [NOUN | VERB | ADJECTIVE] (string)
  -m, --morph         morph type [IPA | JUMAN] (string [=IPA])
  -?, --help          print this message./main adjective antonym target_pos < text_processed_with_jdepp

e.g., ./main -a 大きい -n "" -p ADJECTIVE < foo.txt
```

`target_pos` は，以下の 3 つから選べます．

| target\_pos | 意味                |
|:------------|:--------------------|
| NOUN        | 名詞                |
| VERB        | 動詞                |
| ADJECTIVE   | 形容詞 (と形容動詞) |

`maroph` は，以下の 3 つから選べます．

| maroph | 意味                |
|:-------|:--------------------|
| IPA    | IPA 辞書形態素      |
| JUMAN  | JUAMN 辞書形態素    |

#### 入力
入力として [J.DepP - C++ implementation of Japanese Dependency Parsers]( https://goo.gl/6iF0mD ) によって処理された文を想定しています．

NOTE: 現在の実装では，特に係り受けを使う必要はないのですが，節に分かれた形で出力されると処理が楽という実装上の都合で利用しています．


#### 出力
出力は標準出力に吐かれます．

以下のようなフォーマットです (8 行目以降はテキストに含まれる対象品詞の数だけ繰り返し)．

```
 1| ADJ, adjective
 2| ANT, antonym
 3| TAG, tag
 4| TOTAL_WORDS, total_words
 5| VOCABULARY, vocab
 6| POS_OCCURRENCES, pos_occurrences
 7| NEG_OCCURRENCES, neg_occurrences
 8| OC, target0, 100
 9| P_COOC, target0, 10
10| N_COOC, target0, 10
11| OC, target1, 200
12| ...
```

e.g.,


```sh
$ echo "今日は良い天気ですね" | jdepp 2> /dev/null | ./main -a 良い -n 悪い -p NOUN
ADJ,良い
ANT,悪い
TAG,NOUN
TOTAL_WORDS,6
VOCABULARY,6
POS_OCCURRENCES,1
NEG_OCCURRENCES,0
OC,天気,1
P_COOC,天気,1
N_COOC,天気,0
OC,今日,1
P_COOC,今日,1
N_COOC,今日,0
```
