#!/usr/bin/env ruby
require 'optparse'
require 'csv'
require 'pp'
require 'fileutils'
require_relative 'lister'

params = ARGV.getopts('', 'input:', 'k:10', 'min-occurrence:1', 'output-path:')

if !params['input']
    STDERR.puts "Usage: #{__FILE__} --input counted.csv -k (default: 10) --min-occurrence (default: 1) [--output-path]"
    STDERR.puts "e.g., #{__FILE__} -i counted.csv -k 20 -m 5 -o ./out"
    exit 1
end

input = params['input']
k = params["k"].to_i
m = params["min-occurrence"].to_i

STDERR.puts "input file: #{input}"
STDERR.puts "k: #{k}"
STDERR.puts "min occurrence: #{m}"

lister = Lister.new(input)
lister.min_oc = m

STDERR.puts "reading #{input}..."
lister.read
STDERR.puts "read!"

STDERR.puts "calculating so-scores..."
lister.calc_so_scores
STDERR.puts "calculated!"

# lister.output

STDERR.puts
STDERR.puts "===== output ====="
STDERR.puts "adjective: #{lister.adjective} <-> antonym: #{lister.antonym}"
STDERR.puts "tag: #{lister.tag}"
STDERR.puts "total_words: #{lister.total_words}, vocabulary: #{lister.vocabulary}"
STDERR.puts "pos_occurrences: #{lister.pos_oc}, neg_occurrences: #{lister.neg_oc}"
STDERR.puts
STDERR.puts "K: #{k}"
STDERR.puts "ignored: #{lister.ignored}"

STDERR.puts "best  K: "
best_k = lister.best_k(k)
PP.pp best_k, STDERR

STDERR.puts "worst K: "
worst_k = lister.worst_k(k)
PP.pp worst_k, STDERR


# output {{{
NEGATIVE_SUFFIX = "_NEG"
def remove_suffix(str)
    return str.gsub(NEGATIVE_SUFFIX, "")
end

def exchange_negatives(best, worst)
    new_best = []
    new_worst = []

    best.each do |target, _|
        if target.end_with?(NEGATIVE_SUFFIX)
            new_worst << remove_suffix(target)
        else
            new_best << target
        end
    end

    worst.each do |target, _|
        if target.end_with?(NEGATIVE_SUFFIX)
            new_best << remove_suffix(target)
        else
            new_worst << target
        end
    end

    return new_best, new_worst
end

def remove_originals(list, lister)
    return list.reject{|item| item == lister.adjective || item == lister.antonym }
end

unless params["output-path"].nil?
    out = params["output-path"]

    BEST_PATH  = "best_k.txt"
    WORST_PATH = "worst_k.txt"

    FileUtils.mkdir_p(out)
    STDERR.puts "output_path:   best  -> #{out}/#{BEST_PATH}"
    STDERR.puts "               worst -> #{out}/#{WORST_PATH}"

    # clean up lists
    new_best, new_worst = exchange_negatives(best_k, worst_k)
    new_best  = remove_originals(new_best, lister)
    new_worst = remove_originals(new_worst, lister)

    File.write("#{out}/#{BEST_PATH}",  new_best.join("\n"))
    File.write("#{out}/#{WORST_PATH}", new_worst.join("\n"))
end
# /output }}}
