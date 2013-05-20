/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <algorithm>

#include "Tokenizer.h"


namespace cutil {

inline
ConfigFileReader::ConfigFileReader() throw ()
  : eof_flag( true ) {}

inline
ConfigFileReader::ConfigFileReader( const std::string &filename ) throw () {
  open( filename );
}


inline
ConfigFileReader& ConfigFileReader::operator++() throw () {
  section = "";
  config.clear();
  
  if ( fstack.empty() ){
    eof_flag = true;
    return *this;
  }

  bool in_section = false;
  
  // 次のsectionまで読み込む.
  while ( !fstack.empty() ) {
    std::pair< std::string, std::ifstream::pos_type > &finfo = fstack.back();
    std::ifstream inf( finfo.first.c_str() );    
    inf.seekg( finfo.second );

    while ( true ) {
      if ( !inf ) {
        inf.close();
        fstack.pop_back();
        break;
      }

      std::string line;
      
      std::ifstream::pos_type prev = inf.tellg();
      std::getline( inf, line );
      
      // コメントを削除する.
      line = line.substr( 0, line.find_first_of( "#" ) );
      {  
        std::size_t
          fc = line.find_first_not_of( " \t" ),
          bc = line.find_last_not_of( " \t" );
      
        if ( fc != std::string::npos ) {
          if ( line[ fc ] == '[' && line[ bc ] == ']' ) { // Sectionフィールドを判定, 取得.
            if ( !in_section ) {
              fc = line.find_first_not_of( " \t\"", fc + 1 );
              bc = line.find_last_not_of( " \t\"", bc - 1 );
              section = line.substr( fc, bc - fc + 1 );
              in_section = true;
              continue; // 次の行へ.
            }
            else {        // 前の行の位置を保存してreturn.
              finfo.second = prev;
              inf.close();
              return *this;
            }
          }
          else if ( fc == line.find( "%include", fc ) ) { // ファイルの展開
            fc = line.find_first_not_of( " \t\"", fc + 8 );
            bc = line.find_last_not_of( " \t\"", bc );

            if ( bc - fc >= 0 ) {  // 親ファイルの位置を保存し,ファイルスタックに積む.

              std::string fname = line.substr( fc, bc - fc + 1 );
              
              // 循環参照の禁止
              std::deque< std::pair< std::string, std::ifstream::pos_type > >::const_iterator
                it = fstack.begin(),
                endIt = fstack.end();
              while ( it != endIt ) {
                if ( it->first == fname ) {
                  break;
                }
                ++it;
              }
              
              if ( it == endIt ) { // 現在スタック内にファイルが無いなら良い.
                finfo.second = inf.tellg();
                inf.close();
                
                fstack.push_back( std::pair< std::string, std::ifstream::pos_type >( fname, 0 ) );
                break;   // 子ファイルを読むループへ
              }
            }
            
            continue; // 次の行へ.
          }
        }
      }
      
      
      // sectionが決まって, 通常の行を処理.
      Tokenizer tok( line, ";" );
      
      while ( !tok.isEmpty() ) {
        std::string buf = *tok;
        
        // 本体のkey valueを取得.
        // 最初の= を探して, 左辺値と右辺値に分割.
        std::size_t ec = buf.find_first_of( "=" );
        if ( ec == 0 ||
             ec == std::string::npos ) {
          ++tok;
          continue;
        }
        
        // 左辺値と右辺値の端の空白を削除.
        
        std::size_t 
          k_fc = buf.find_first_not_of( " \t" ),
          k_bc = buf.find_last_not_of( " \t", ec - 1 ),
          k_len = k_bc - k_fc + 1;
        
        if ( k_len <= 0 ) {
          ++tok;
          continue;
        }
        std::string key = buf.substr( k_fc, k_len );
        
        
        std::string value = "";
        
        if ( ec < buf.size() - 1 ) {
          std::size_t v_fc = buf.find_first_not_of( " \t\"", ec + 1 );
          
          if ( v_fc != std::string::npos ) {
            std::size_t 
              v_bc = buf.find_last_not_of( " \t\"" ),
              v_len = v_bc - v_fc + 1;
            
            if ( v_len > 0 ) {
              value = buf.substr( v_fc, v_len );

              // \を削除.
              value.erase( std::remove( value.begin(), value.end(), '\\' ), value.end() );
            }
          }
        }
        
        config.insert( std::pair< std::string, std::string >( key, value ) );
        ++tok;
      }
    }
  }

  return *this;
}

inline
std::map< std::string, std::string >& ConfigFileReader::operator*() throw () {
  return config;
}

inline
std::map< std::string, std::string >* ConfigFileReader::operator->() throw () {
  return &config;
}

inline
void ConfigFileReader::open( const std::string &filename ) throw () {
  fstack.push_back( std::pair< std::string, std::ifstream::pos_type >( filename, 0 ) );
  eof_flag = false;
  section = "";
  config.clear();
  ++(*this);
}

inline
const std::string& ConfigFileReader::getSection() const throw () {
  return section;
}
  
inline
bool ConfigFileReader::isEmpty() const throw () {
  return eof_flag;
}

}
