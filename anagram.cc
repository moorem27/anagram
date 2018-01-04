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
                std::set<std::string> v{};
                v.insert( place_holder );
                word_map[ place_holder.size() ] = v;
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
            for( const auto& a : find_anagrams( place_holder, word_map ) ) {
                output_file << a;
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
        std::transform( search_word.begin(), search_word.end(), search_word.begin(), ::tolower );
        auto word_map = create_word_map( WORD_FILE );
        auto anagrams = find_anagrams( search_word, word_map );

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
