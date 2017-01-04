PMI box
===

Pointwise Mutual Information を使う自然言語処理の日本語用ツールキットです．

# [counter](./counter)
**極性を考慮して**，与えられた形容詞 (とその対義語) と，
対象とする品詞に属する単語との共起回数を数え上げるプログラムです．
詳しくは [README](./counter/README.md) をご覧ください．

# [reducer](./reducer)
[counter](./counter) が出力した結果を複数受け取ってまとめるプログラムです．
詳しくは [README](./reducer/README.md) をご覧ください．

# [lister](./lister)
[counter](./counter) が出力した結果をもとに
Turney の極性指標 (SO 値) に基いての k 個の best / worst の
単語を出力するプログラムです．
詳しくは [README](./lister/README.md) をご覧ください．
