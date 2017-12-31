#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

namespace {
    const std::string WORD_FILE = "/usr/share/dict/words";
}

std::map<int, std::vector<std::string>> create_word_map( const std::string& file_path ) {
    std::ifstream file( file_path, std::ios::binary );
    std::string place_holder{};
    std::map<int, std::vector<std::string>> word_map{};

    if( file.is_open() ) {
        while( std::getline( file, place_holder ) ) {
            if( word_map.find( place_holder.size() ) == word_map.end() ) {
                std::vector<std::string> v{};
                v.push_back( place_holder );
                word_map[ place_holder.size() ] = v;
            } else {
                word_map[ place_holder.size() ].push_back( place_holder );
            }

            place_holder.clear();
        }
    } else {
        std::cout << "File is not open" << std::endl;
    }

    file.close();

    return word_map;
}

std::vector<std::string> find_anagram( const std::string& word, std::map<int, std::vector<std::string>>& word_map ) {
    std::vector<std::string> anagrams{};

    for( const auto& map_word : word_map[ word.size() ] ) {
        if( std::is_permutation( word.begin(), word.end(), map_word.begin() ) && map_word.compare( word ) != 0 )
            anagrams.push_back( map_word );
    }

    return anagrams;
}


int main( int argc, char* argv[] ) {

    if( argc == 2 ) {
        const std::string search_word = std::string{ argv[ 1 ] };
        std::cout << "Searching for anagrams of the word: " << search_word << std::endl;
        auto word_map = create_word_map( WORD_FILE );
        auto anagrams = find_anagram( search_word, word_map );

        if( anagrams.size() == 0 ) {
            std::cout << "Nothing found!" << std::endl;
            return 0;
        }

        for( const auto& a : anagrams ) {
            std::cout << a << std::endl;
        }
    } else {
        std::cout << "Please enter one command line argument" << std::endl;

        return -1;
    } 

    return 0;
}
