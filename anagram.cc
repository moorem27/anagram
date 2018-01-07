#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

namespace {
    const std::string WORD_FILE = "/usr/share/dict/words";
}

std::map<int, std::set<std::string>> create_word_map( const std::string& file_path ) {
    std::ifstream word_file( file_path, std::ios::binary );
    std::string place_holder{};
    std::map<int, std::set<std::string>> word_map{};

    if( word_file.is_open() ) {
        while( std::getline( word_file, place_holder ) ) {
            std::transform( place_holder.begin(), place_holder.end(), place_holder.begin(), ::tolower );
            if( word_map.find( place_holder.size() ) == word_map.end() ) {
                std::set<std::string> word_set{};
                word_set.insert( place_holder );
                word_map[ place_holder.size() ] = word_set;
            } else {
                word_map[ place_holder.size() ].insert( place_holder );
            }

            place_holder.clear();
        }
    } else {
        std::cout << "File is not open" << std::endl;
    }

    word_file.close();

    return word_map;
}

std::set<std::string> find_anagrams( std::string& word, const std::string& file_path ) {
    std::ifstream word_file( file_path, std::ios::binary );
    std::string place_holder{};
    std::set<std::string> anagrams{};
    std::transform( word.begin(), word.end(), word.begin(), ::tolower );

    if( word_file.is_open() ) {
        while( std::getline( word_file, place_holder ) ) {
            std::transform( place_holder.begin(), place_holder.end(), place_holder.begin(), ::tolower );
            if( place_holder.size() == word.size() && std::is_permutation( word.begin(), word.end(), place_holder.begin() ) ) {
                if( place_holder.compare( word ) != 0 ) 
                    anagrams.insert( place_holder );
            }
            place_holder.clear();
        }
    }

    word_file.close();

    return anagrams;
}

std::set<std::string> find_anagrams( const std::string& word, std::map<int, std::set<std::string>>& word_map ) {
    std::set<std::string> anagrams{};

    for( const auto& map_word : word_map[ word.size() ] ) {
        if( std::is_permutation( word.begin(), word.end(), map_word.begin() ) && map_word.compare( word ) != 0 )
            anagrams.insert( map_word );
    }

    return anagrams;
}

// Call this function in main: 
// 
// get_all_anagrams( WORD_FILE );
//
// to create a file of words and their
// anagrams, based on /usr/share/dict/words
void get_all_anagrams( const std::string& word_file_path ) {
    std::ifstream word_file( word_file_path, std::ios::binary );
    std::ofstream output_file("anagrams.txt");

    auto word_map = create_word_map( word_file_path );   
    std::string place_holder{};
 
    if( word_file.is_open() ) {
        while( std::getline( word_file, place_holder ) ) {
            output_file << place_holder;
            output_file << " ";
            std::transform( place_holder.begin(), place_holder.end(), place_holder.begin(), ::tolower );
            for( const auto& anagram : find_anagrams( place_holder, word_map ) ) {
                output_file << anagram;
                output_file << " ";
            }
            output_file << "\n";
            place_holder.clear();
        }
    }

    word_file.close();
    output_file.close();
}

int main( int argc, char* argv[] ) {
    if( argc == 2 ) {
        std::string search_word = std::string{ argv[ 1 ] };
        auto anagrams = find_anagrams( search_word, WORD_FILE );

        if( anagrams.size() == 0 ) {
            std::cout << "Nothing found!" << std::endl;
            return 0;
        }

        for( const auto& anagram : anagrams ) {
            std::cout << anagram << std::endl;
        }
    } else {
        std::cout << "Please enter one command line argument" << std::endl;

        return -1;
    } 

    return 0;
}
