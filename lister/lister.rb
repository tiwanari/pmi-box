require 'csv'


class Lister
    attr_accessor :min_oc
    attr_reader :ignored, :adjective, :antonym, :tag, :total_words, :vocabulary, :pos_oc, :neg_oc

    ADJ_STR = "ADJ"
    ANT_STR = "ANT"
    TAG_STR = "TAG"
    TOTAL_WORDS_STR = "TOTAL_WORDS"
    VOCABULARY_STR = "VOCABULARY"
    POS_OCCURRENCES_STR = "POS_OCCURRENCES"
    NEG_OCCURRENCES_STR = "NEG_OCCURRENCES"
    OCCURRENCES_STR = "OC"
    POS_CO_OCCURRENCES_STR = "P_COOC"
    NEG_CO_OCCURRENCES_STR = "N_COOC"

    def initialize(input)
        @input = input
        @words = []
        @raw_data = { OCCURRENCES_STR => {}, POS_CO_OCCURRENCES_STR => {}, NEG_CO_OCCURRENCES_STR => {},}
        @so_values = {}
        @ignored = 0
        @min_oc = 1
    end

    def read_adjective(row)
        assert_eq(ADJ_STR, row[0])
        @adjective = row[1]
    end

    def read_antonym(row)
        assert_eq(ANT_STR, row[0])
        @antonym = row[1]
    end

    def read_tag(row)
        assert_eq(TAG_STR, row[0])
        @tag = row[1]
    end

    def read_total_words(row)
        assert_eq(TOTAL_WORDS_STR, row[0])
        @total_words = row[1].to_i
    end

    def read_vocab(row)
        assert_eq(VOCABULARY_STR, row[0])
        @vocabulary = row[1].to_i
    end

    def read_pos_oc(row)
        assert_eq(POS_OCCURRENCES_STR, row[0])
        @pos_oc = row[1].to_i
    end

    def read_neg_oc(row)
        assert_eq(NEG_OCCURRENCES_STR, row[0])
        @neg_oc = row[1].to_i
    end

    def read
        reader = CSV.open(@input, 'r')
        read_adjective(reader.gets)     # 1
        read_antonym(reader.gets)       # 2
        read_tag(reader.gets)           # 3
        read_total_words(reader.gets)   # 4
        read_vocab(reader.gets)         # 5
        read_pos_oc(reader.gets)        # 6
        read_neg_oc(reader.gets)        # 7

        reader.each_slice(3) do |rows|
            assert_eq(OCCURRENCES_STR, rows[0][0])
            assert_eq(POS_CO_OCCURRENCES_STR, rows[1][0])
            assert_eq(NEG_CO_OCCURRENCES_STR, rows[2][0])

            if rows[0][2].to_i < min_oc
                @ignored += 1
                next
            end

            @raw_data[OCCURRENCES_STR][rows[0][1]] = rows[0][2].to_i
            @raw_data[POS_CO_OCCURRENCES_STR][rows[1][1]] = rows[1][2].to_i
            @raw_data[NEG_CO_OCCURRENCES_STR][rows[2][1]] = rows[2][2].to_i
        end
    end

    def so(word)
        # x: word, y: adjective (or negative)
        # e^PMI(x, y) = p(x, y) / (p(x) p(y))
        # = [co-occurrence(x, y) / co-occurrence(*, *)]
        #      / [(count(x) / count(*)) * (count(y) / count(*))]
        #
        # so-score = PMI(x, y_pos) - PMI(x, y_neg)
        # e.g. PMI(タンス, 重いor軽くない) - PMI(タンス, 軽いor重くない)
        #
        # PMI(x, y_pos) - PMI(x, y_neg)
        # = log(co-occurrence(x, y_pos)) - log(count(y_pos))
        #   - [log(co-occurrence(x, y_neg)) - log(count(y_neg))]
        # and add 1 smoothing (each denominator is canceled, so it's easy)
        # -> log(co-occurrence(x, y_pos) + 1) - log(count(y_pos) + 1)
        #   - [log(co-occurrence(x, y_neg) + 1) - log(count(y_neg) + 1)]
        (Math.log(@raw_data[POS_CO_OCCURRENCES_STR][word] + 1) - Math.log(@pos_oc + 1)) \
        - (Math.log(@raw_data[NEG_CO_OCCURRENCES_STR][word] + 1) - Math.log(@neg_oc + 1))
    end

    def calc_so_scores
        @raw_data[OCCURRENCES_STR].each do |k, v|
            @so_values[k] = so(k)
        end
    end

    def best_k(k)
        @so_values.sort_by {|_,v| -v}.first(k)
    end

    def worst_k(k)
        @so_values.sort_by {|_,v| v}.first(k)
    end

    def output
        p @so_values
    end

private
    def assert_eq(expected, actual)
        raise "(ACTUAL) #{actual} != #{expected} (EXPECTED)" if actual != expected
    end
end
