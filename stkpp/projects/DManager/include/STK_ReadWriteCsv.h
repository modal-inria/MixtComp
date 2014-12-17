/*--------------------------------------------------------------------*/
/*     Copyright (C) 2004-2007  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA

    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
*/

/*
 * Project:  stkpp::DManager
 * Purpose:  Declaration of the class ReaWriteCsv.
 * Author:   Serge Iovleff, S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)
 *
 **/

/** @file STK_ReadWriteCsv.h
 *  @brief In this file we define the class TReadWriteCsv.
 *
 *  This class allow to handle csv files in various ways.
 **/

#ifndef STK_READWRITECSV_H
#define STK_READWRITECSV_H

#include <iomanip>
#include "STK_Variable.h"

namespace STK
{

template<typename Type> class TReadWriteCsv;

typedef class TReadWriteCsv<String> ReadWriteCsv;

/** The STK::Csv namespace contains the constants used in the DManager
 *  project when using csv files.
 **/
namespace Csv
{
  /** @ingroup DManager
   *  read_names_ default value */
  static const bool   DEFAULT_READNAMES = true;
  /** @ingroup DManager
   *  with_mapping_ default value */
  static const bool   DEFAULT_MAPPING = false;
  /** @ingroup DManager
   *  reserve_ default value*/
  static const int DEFAULT_RESERVE = int(0x0FFFF);
  /** @ingroup DManager
   *  default field delimiter in a file */
  static const String DEFAULT_DELIMITER  = _T(",");
  /** @ingroup DManager
    * @brief some errors messages in TReadWriteCsv. */
  static const String ERRORCODES[] =
  {
    _T(""), // no message
    _T("An unknown error occurred!"),
    _T("Variable name not found!"),
    _T("Filename not set!"),
    _T("File not found!"),
    _T("The Number of names is different from the Number of Data Columns!")
   };
  /** @ingroup DManager
   *  TODO: These flags determine the behavior of the reading methods.
   **/
  enum readflags
  {
    RW_APPEND_DATA    = 0x00000004,
    RW_REPLACE_DATA   = 0x00000008
  };
}

/** @ingroup DManager
 *  @brief the TReadWriteCsv class : allow to write and/or to read a csv
 *  file.
 *
 *  It is possible to merge two csv files and to extract subregion of
 *  the file too. All data are stored in a Type format. if there exists mixed
 *  types in the file you want to read/write, used the String format and use
 *  the import/export utilities classes @sa ExportToCsv, ImportFromCsv.
 *
 *  It is possible to use a mapping in order to write/read the data using
 *  an other encoding.
 **/
template<typename Type>
class TReadWriteCsv
{
  public:
    typedef Variable<Type> Var;
    /** The default constructor. Instantiates an instance of TReadWriteCsv
     *  and initialize data members to default values.
     **/
    inline TReadWriteCsv() : file_name_()
                           , with_names_(Csv::DEFAULT_READNAMES)
                           , with_mapping_(Csv::DEFAULT_MAPPING)
                           , delimiters_(Csv::DEFAULT_DELIMITER)
                           , reserve_(Csv::DEFAULT_RESERVE)
                           , msg_error_()
                           , nbVars_(0)
                           , nbRows_(0)

    {}
    /** Constructor with a specified file name.
     *  Instantiates an instance of TReadWriteCsv with the specified read flags.
     *  @param file_name name of the file to read/write
     *  @param read_names true if we want read/write the names of the
     *  variables at the first line of the file
     *  @param delimiters The delimiters used in the fileo(if read) or the
     *  delimiter to use (if write)
     **/
    inline TReadWriteCsv( std::string const& file_name
                        , bool read_names = Csv::DEFAULT_READNAMES
                        , String const& delimiters = Csv::DEFAULT_DELIMITER
                        )
                        : file_name_(file_name)
                        , with_names_(read_names)
                        , with_mapping_(Csv::DEFAULT_MAPPING)
                        , delimiters_(delimiters)
                        , reserve_(Csv::DEFAULT_RESERVE)
                        , msg_error_()
                        , nbVars_(0)
                        , nbRows_(0)
    {}
    /** Copy constructor. Instantiates an instance of TReadWriteCsv with
     *  the contents of another TReadWriteCsv.
     *  @param rw the TReadWriteCsv to copy
     **/
    inline TReadWriteCsv(TReadWriteCsv const& rw) { *this = rw;}
    /** destructor. */
    inline ~TReadWriteCsv() {}
    /** reset all fields to default value and reclaims any allocated memory. */
    void clear()
    {
      msg_error_.clear();
      str_data_.clear();
      with_names_   = Csv::DEFAULT_READNAMES;
      with_mapping_ = Csv::DEFAULT_MAPPING;
      delimiters_   = Csv::DEFAULT_DELIMITER;
      reserve_      = Csv::DEFAULT_RESERVE;
    }
    /**  @return the index of the first variable (should be baseIdx). */
    inline int begin() const { return str_data_.begin(); }
    /** @return the ending index of the variables */
    inline int end() const { return str_data_.end(); }
    /**@return The current number of variables of the TReadWriteCsv */
    inline int size() const { return str_data_.size(); }

    /**@return The range of the variables of the TReadWriteCsv */
    inline Range cols() const { return str_data_.range(); }
    /**  @return the index of the first variable (should be baseIdx). */
    inline int beginCols() const { return str_data_.begin(); }
    /** @return the ending index of the variables */
    inline int endCols() const { return str_data_.end(); }
    /**@return The current number of variables of the TReadWriteCsv */
    inline int sizeCol() const { return str_data_.size(); }

    /** @param icol index of the variable
     *  @return the first index in the column @c icol
     **/
    inline int firstRow( int icol) const { return str_data_.at(icol).begin();}
    /** @param icol index of the column
     *  @return the ending index of the column @c icol
     **/
    inline int endRow( int icol) const { return str_data_.at(icol).end();}
    /** @param icol index of the column
     *  @return the last index in the column @c icol
     **/
    inline int lastRow( int icol) const { return str_data_.at(icol).lastIdx();}
    /** @param icol index of the column
     *  @return the number of element in the column @c icol
     **/
    inline int sizeRow( int icol) const { return str_data_.at(icol).size();}
    /** @return the first index of the samples. */
    int beginRows() const
    {
      if (size()<= 0) return baseIdx;
      int retVal = firstRow(begin());
      for (int i=begin()+1; i<=lastIdx(); i++)
      { retVal = std::min(retVal, firstRow(i));}
      return retVal;
    }
    /** @return the ending index of the samples. */
    int endRows() const
    {
      if (size()<= 0) return baseIdx;
      int retVal = lastRow(begin());
      for (int i=begin()+1; i<end(); i++)
      { retVal = std::max(retVal, endRow(i));}
      return retVal;
    }
    /** @return the size of the samples. */
    int sizeRows() const
    {
      if (size()<= 0) return 0;
      int retVal = sizeRow(begin());
      for (int i=begin()+1; i<end(); i++)
      { retVal = std::max(retVal, sizeRow(i));}
      return retVal;
    }
    /** @return the last index of the samples. */
    int lastIdxRows() const
    {
      if (size()<= 0) return baseIdx-1;
      int retVal = lastRow(begin());
      for (int i=begin()+1; i<=lastIdx(); i++)
      { retVal = std::max(retVal, lastRow(i));}
      return retVal;
    }
    /**@return The range of the samples of the TReadWriteCsv */
    inline Range rows() const { return Range(beginRows(), lastIdxRows(), 0); }

    /** @return the index of the last variable */
    inline int lastIdx() const { return str_data_.lastIdx(); }
    /** @return the index of the last variable */
    inline int lastIdxCols() const { return str_data_.lastIdx(); }

    /** @return the last error encountered */
    inline String const& error() const { return msg_error_; }
    /**@return the delimiters used in the Csv file*/
    inline String const& delimiters() const { return delimiters_; }
    /** @return with_names value */
    inline bool withNames() const { return with_names_; }
    /** Sets the delimiters to use for parsing data (delimiters_ is mutable).
     *  @param delimiters delimiters to use
     **/
    inline void setDelimiters( String const& delimiters) const { delimiters_ = delimiters; }
    /** Sets the with_names_ value for reading/writting variables names
     *  @param with_names @c true if we want to read the names of the variables
     **/
    inline void setWithNames( bool with_names) const { with_names_ = with_names; }
    /** Sets the with_mapping_ value for reading/writting variables names
     *  @param with_mapping @c true if we want to read the data using a map
     **/
    inline void setWithMapping( bool with_mapping) const { with_mapping_ = with_mapping; }
    /** Sets the reserve value for data storage (reserve_ is mutable).
     *  @param reserve number of place to reserve
     **/
    inline void setReserve( int const& reserve) const { reserve_ = reserve; }
    /** Set mapping to used when reading */
    inline void setInMapping( std::map<String, Type> const& imapping)
    { imapping_ = imapping;}
    /** Set mapping to used when writting */
    inline void setOutMapping( std::map<String, Type> const& omapping)
    { omapping_ = omapping;}
         /** Set the variable name @c name at the specified index.
     *  @param icol index of the variable
     *  @param name name of the variable to set
     *  @return @c true if successful, @c false if an error is encountered.
     **/
    bool setName( int const& icol, String const& name)
    {
      try
      {
        str_data_.at(icol).setName(name);
        return true;
      }
      catch( Exception const& e) { msg_error_ = e.error(); }
      catch(...)  { msg_error_ = Csv::ERRORCODES[1]; }
      return false;
    }

    /** @return a reference of the value specified by the given coordinates.
     *  @param iCol index of the column
     *  @param iRow index of the row
     **/
    inline Type& operator()( int const& iRow, int const& iCol)
    { return str_data_.elt(iCol).elt(iRow);}
    /** @return a constant reference of the value specified by the given coordinates.
     *  @param iCol index of the column
     *  @param iRow index of the row
     **/
    inline Type const operator()( int const& iRow, int const& iCol) const
    { return str_data_.elt(iCol).elt(iRow);}
    /** @return the variable with the index icol.
     *  @param icol index of the col
     **/
    inline Var* clone(int icol) const { return str_data_.elt(icol).clone(); }
    /** @return the variable with the index icol.
     *  @param j index of the column
     **/
    inline Var& var(int j) { return str_data_.elt(j); }
    /** @return a constant reference of the icol variable.
     *  @param icol index of the col
     **/
    inline Var const& var(int icol) const { return str_data_.elt(icol); }
    /** @return the element with the index icol.
     *  @param icol index of the col
     **/
    inline Var& at(int icol) { return str_data_.at(icol); }
    /** @return a constant reference of the icol variable.
     *  @param icol index of the col
     **/
    inline Var const& at(int icol) const { return str_data_.at(icol); }
    /** @return a reference of the icol variable
     *  @param icol index of the col
     **/
    inline Var& operator[](int const& icol) { return str_data_[icol]; }
    /** @ return a const reference to the value at the specified location.
     *  @param icol index of the col
     **/
    inline Var const operator[](int const& icol) const { return str_data_[icol]; }
   /** @return the first variable. */
    inline Var& front() { return str_data_.at(begin());}
    /** @return the first variable (const). */
    inline Var const& front() const { return str_data_.at(begin());}
    /** @return the last variable. */
    inline Var& back() { return str_data_.at(lastIdx());}
    /** @return the last variable (const). */
    inline Var const& back() const { return str_data_.at(lastIdx());}
    /** Attempts to add a column with the values contained in data.
     *  @param data the column to push back
     *  @return @c true if successful, @c false if an error is encountered.
     **/
    bool push_back( Var const& data = Var())
    {
      try
      {
        str_data_.push_back(data);
        //str_data_.back().reserve(reserve_);
        return true;
      }
      catch(const Exception& e) { msg_error_ = e.error(); }
      catch(...) { msg_error_ = Csv::ERRORCODES[1]; }
      return false;
    }
    /** Attempts to add a column with the values contained in data.
     *  @param data the column to push front
     *  @return @c true if successful, @c false if an error is encountered.
     **/
    bool push_front( Var const& data = Var())
    {
      try
      {
        str_data_.push_front(data);
        return true;
      }
      catch(const Exception& e) { msg_error_ = e.error(); }
      catch(...) { msg_error_ = Csv::ERRORCODES[1]; }
      return false;
    }
    /** resize the container str_data_.
     *  @param sizeCols number of columns
     *  @param sizeRows number of rows
     **/
    inline void resize( int sizeRows, int sizeCols)
    {
      resizeCols(sizeCols);
      resizeRows(sizeRows);
    }
    /** Attempts to append a data to the variable specified by icol.
     *  @param icol index of the col
     *  @param value value to set
     *  @return @c true if successful, @c false if an error is encountered.
     **/
    bool appendData( int const& icol, Type const& value)
    {
      try
      {
        if (Arithmetic<Type>::isNA(value))
          str_data_[icol].push_back(Arithmetic<Type>::NA());
        else
          str_data_[icol].push_back(value);
        return true;
      }
      catch(const Exception& e) { msg_error_ = e.error(); }
      catch(...) { msg_error_ = Csv::ERRORCODES[1]; }
      return false;
    }
    /** Deletes the variable whose index is icol from a TReadWriteCsv.
     *  @param icol index of the column to erase
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool eraseColumn(int const& icol)
    {
      try
      {
        str_data_.erase(icol);
        return true;
      }
      catch( Exception const& e) { msg_error_ = e.error(); }
      catch(...) { msg_error_ = Csv::ERRORCODES[1]; }
      return false;
    }
    /** Assigns a TReadWriteCsv equal to another TReadWriteCsv.
     *  @param rw the TReadWriteCsv to copy
     **/
    TReadWriteCsv& operator=( TReadWriteCsv const& rw)
    {
      file_name_     = rw.file_name_;
      with_names_    = rw.with_names_;
      delimiters_    = rw.delimiters_;
      reserve_       = rw.reserve_;
      msg_error_     = rw.msg_error_;
      str_data_      = rw.str_data_;
      nbVars_        = rw.nbVars_;
      nbRows_        = rw.nbRows_;
      return *this;
    }
    /** Appends a TReadWriteCsv to another TReadWriteCsv.
     *  @param rw the TReadWriteCsv to append
     **/
    TReadWriteCsv& operator+=( TReadWriteCsv const& rw)
    {
      for ( int i=rw.begin(); i<=rw.lastIdx(); i++)
      { str_data_.push_back(rw.str_data_[i]);}
      return *this;
    }
    /** Combines TReadWriteCsv(s)
     *  @param rw the TReadWriteCsv to add
     **/
    TReadWriteCsv operator+( TReadWriteCsv const& rw) const
    { return TReadWriteCsv((*this)) += rw;}
    /** Reads the default file with the specified read flags.
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    inline bool read() { return read(file_name_);}
    /** Reads the default file with the specified read flags using a map.
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    inline bool read(std::map<String, Type> const& mapping)
    { return read(file_name_);}
    /** Reads the specified file with the specified read flags.
     *  @param file_name name of the file to read
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool read(std::string const& file_name)
    {
      try
      {
        // update current file_name
        file_name_ = file_name;
        // input file stream
        ifstream inFile;
        // open file
        inFile.open(file_name.c_str(),std::ios::in);
        // check error
        if (inFile.rdstate() & std::ios::failbit)
        {
          inFile.close();
          msg_error_ = Csv::ERRORCODES[4];
          msg_error_ += "\nFile: " + file_name;
          return false;
        }
        read(inFile);                  // and read it
        inFile.close();
        // return
        return true;
      }
      catch(const Exception& e)
      { msg_error_ = e.error();}
      catch(...)
      { msg_error_ = Csv::ERRORCODES[1];}
      return false;
    }
    /** Reads the specified input stream with the specified read flags.
     *  @param inBuffer name of the stream to read
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool read( istream& inBuffer);
    /** write to the default file with the specified write flags.
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    inline bool write() { return write(file_name_);}
    /** Attempts to write the TReadWriteCsv to the location specified by
     *  file_name using the delimiters specified by delimiters_.
     *  @param file_name name of the file to write
     *  @return  @c true if successful, @c false if an error is encountered.
     **/
    bool write( std::string const& file_name) const
    {
      file_name_  = file_name;
      try
      {
        ofstream os(file_name.c_str());
        writeSelection( os, beginRows(), lastIdxRows(), begin(), lastIdx());
        os.close();
        return true;
      }
      catch(const Exception& e) { msg_error_ = e.error(); }
      catch(...) { msg_error_ = Csv::ERRORCODES[1]; }
      return false;
    }
    /** Write the csv to an output stream.
     *  @param os the output stream
     **/
    void write( ostream& os) const
    { writeSelection(os, beginRows(), lastIdxRows(), begin(), lastIdx());}
    /** Write to output stream a selection based on the coordinates
     *  passed (Think of it as highlighting cells in Excel).
     *  @param os the output stream
     *  @param top the top index
     *  @param bottom th bottom index
     *  @param left the left index
     *  @param right the right index
     **/
    void writeSelection( ostream& os, int top, int bottom, int left, int right) const;

    /** overload operator >> for rw*/
    friend istream& operator>>( istream& in, TReadWriteCsv& rw)
    {
      if (!rw.read(in)) throw Exception(rw.msg_error_);
      return in;
    }
    /** overload operator << for rw */
    friend ostream& operator<<( ostream& os, TReadWriteCsv const& rw)
    {
      try
      { rw.writeSelection( os, rw.beginRows(), rw.lastIdxRows(), rw.begin(), rw.lastIdx());}
      // catch and re-throw any Exceptions
      catch(const Exception& e) { throw e; }
      catch(...) { throw Exception(Csv::ERRORCODES[1]); }
      return os;
    }

  protected:
    /// Name of the Current file read
    mutable std::string file_name_;
    /// Read and Write names of the variables
    mutable bool with_names_;
    /// Read and Write names of the variables
    mutable bool with_mapping_;
    /// Delimiter(s)
    mutable String  delimiters_;
    /// Size of the buffer
    mutable int reserve_;
    /// Contain the last error message
    mutable String msg_error_;
    /** Array of array for the data.*/
    Array1D< Var > str_data_;
    /** mapping String -> Type for (input) */
    std::map<String, Type> imapping_;
    /** mapping Type -> String (output) */
    std::map<Type, String> omapping_;
    /** The number of variables we manage */
    int nbVars_;
    /** The number of rows we manage */
    int nbRows_;
    /** Counts the number of columns in a line stored in a String
     *  and return the position of the delimiters and its types.
     *  @param line The String to parse
     *  @param listDelimiters array with the delimiters found in the line
     *  @return the number of field in the line
     **/
    int countFields( String const& line, Array1D<Char>& listDelimiters)
    {
      listDelimiters.clear();
      listDelimiters.reserve(nbVars_);
      // position of the delimiters in the file
      String::size_type idx =0;
      do
      {
        // Find delimiter
        idx = line.find_first_of(delimiters_, idx);
        // if the position of the delimiter char is not found
        if (idx == line.npos) break;
        listDelimiters.push_back(line[idx]);
        ++idx;
      } while(1);
      // last delimiter is the end of line
      listDelimiters.push_back(CHAR_NL);
      listDelimiters.shift(1);
      // return the number of fields
      return listDelimiters.size();
    }
    /** Read the current line from the buffer.
     *  @return the number of field found in the current line
     *  @param inBuffer the input stream to read
     *  @param currentLine the line to read
     *  @param listDelimiters the delimiters between the fields in the current line
     **/
    int readCurrentLine(istream& inBuffer, String& currentLine, Array1D<Char>& listDelimiters)
    {
      if (inBuffer.eof()) return 0;
      int nbField =0;
      do
      {
        currentLine.clear();
        std::getline(inBuffer, currentLine);
        if (currentLine.size() == 0) {nbField = 0;}
        else
        {
          DManager::removeCharBeforeAndAfter(currentLine, CHAR_BLANK);
          nbField = (currentLine.size() == 0)
                  ? 0 : countFields( currentLine, listDelimiters);
        }
      }
      while ((nbField == 0)&&(!inBuffer.eof()));
      return nbField;
    }
    /** Read the names of rhe Csv file. They are stored in the first line of the file.
     *  @param inBuffer the input stream to read
     **/
    void readNames(istream& inBuffer)
    {
      Array1D<Char> listDelimiters;
      String currentLine;
      // Count the number of names of the first line
      int nbField = readCurrentLine(inBuffer, currentLine, listDelimiters);
      if (nbField == 0) return;
      // Declare an input string stream
      istringstream instream(currentLine);
      for(int icol=1; icol<=nbField; icol++)
      {
        // Append a Col
        String name;
        DManager::getField( instream, name, listDelimiters.elt(icol));
        push_back(Var(nbRows_, name));
      }
      // Update the number of var
      nbVars_ = std::max(nbVars_, nbField);
    }
    /** Initial read. Will compute an initial value for nbRows_ and nbVars_ and
     *  eventually read the name of the variables in the first line of the file.
     *  @param inBuffer the input stream to start to read
     **/
    void initRead(istream& inBuffer)
    {
      Array1D<Char>  listDelimiters;
      String currentLine;
      // clear previous ReadCvs if any
      str_data_.clear();
      // get current position in the stream
      std::streampos pos = inBuffer.tellg();
      // Count the number of rows of the input stream
      nbRows_ = DManager::nbEndOfLine(inBuffer);
      setReserve(nbRows_);
      // revert to the current position in the stream
      inBuffer.seekg(pos);
      // initialize the initial number of variables to 0
      nbVars_ = 0;
      // If the names are at the top line
      if (with_names_) { readNames(inBuffer);}
      // get current position
      pos = inBuffer.tellg();
      // count number of fields in the first line
      int nbField = readCurrentLine(inBuffer, currentLine, listDelimiters);
      nbVars_ = std::max(nbVars_, nbField);
      inBuffer.seekg(pos);
    }
    /** Set without check the specified value to the element (icol, irow).
     *  @param icol index of the column
     *  @param irow index of the row
     *  @param value the value to set
     **/
    inline void fastSetData( int const& icol, int const& irow, Type const& value)
    { str_data_.elt(icol).elt(irow) = value;}
    /** resize the @c TReadWriteCsv to the given range of rows.
     *  @param rows the range of the rows
     **/
    inline void resizeRows( Range const& rows)
    {
      for (int iVar=begin(); iVar<=lastIdx(); iVar++ )
      { str_data_.elt(iVar).resize(rows);}
    }
    /** resize the @c TReadWriteCsv to the given range of columns.
     *  @param cols the range of columns to set
     **/
    inline void resizeCols( Range const& cols)
    { str_data_.resize(cols);}
};

/* Reads the specified file with the specified read flags.
 *  @param file_name name of the file to read
 *  @return  @c true if successful, @c false if an error is encountered.
 **/
template <typename Type>
bool TReadWriteCsv<Type>::read( istream& inBuffer)
{
  Array1D<Char> listDelimiters;
  String currentLine;
  try
  {
    initRead(inBuffer);
    resizeCols(nbVars_);
    resizeRows(nbRows_);
    // Read data : loop for all rows
    int currentRow=beginRows()-1, countRows = 0;
    for (; !inBuffer.eof();)
    {
      int nbField = readCurrentLine(inBuffer, currentLine, listDelimiters);
      if (nbField == 0) { break;}
      //  add on the non-existing columns: will be done if (nbField >= nbVars_)
      for (int iField=nbVars_+1; iField<=nbField; iField++)
      { push_back(Var( nbRows_, Arithmetic<Type>::NA(), stringNa));
        nbVars_++;
      }
      // first loop on the existing columns with data
      currentRow++; countRows++;
      istringstream instream(currentLine);
      int icol=begin();
      for (int iField=1 ; iField<=nbField; iField++, icol++)
      {
        String field;
        DManager::getField( instream, field, listDelimiters.elt(iField));
        // append Data to the column
        with_mapping_ ?
            fastSetData(icol, currentRow, imapping_.count(field) ? imapping_.find(field)->second : Arithmetic<Type>::NA())
         :  fastSetData(icol, currentRow, stringToType<Type>(field));
      }
      // Append NA values if the row is not complete
      for (; icol<=lastIdx(); icol++) { fastSetData(icol, currentRow, Arithmetic<Type>::NA());}
    }
    // resize in case there exists too much rows
    resizeRows(Range(countRows));
    return true;
  }
  catch( Exception const& e) { msg_error_ = e.error(); }
  catch(...) { msg_error_ = Csv::ERRORCODES[1]; }
  return false;
}

template <typename Type>
void TReadWriteCsv<Type>::writeSelection( ostream& os, int top, int bottom, int left, int right) const
{
  if ((right < left)|| (bottom < top)) return;
  // create a vector for the format of the output
  Array1D<String::size_type>  format(Range(left, right, 0), 0);
  // for each variable, find the largest size
  for(int iVar=left; iVar<=right; iVar++)
  { format[iVar] = str_data_[iVar].maxLength(with_names_);}
  // write if needed names variables
  if (with_names_)
  {
    for(int iVar=left; iVar<right; iVar++)
    {
      os << std::setw(int(format[iVar])) << std::right
         << Proxy<String>(str_data_.at(iVar).name())
         << delimiters_.at(0);
    }
    os << std::setw(int(format[right])) << std::right
       << Proxy<String>(str_data_[right].name())
       << STRING_NL;
  }
  // write data
  for(int irow = top; irow<=bottom; irow++)
  {
    for(int iVar = left; iVar<right; iVar++)
    {
      try
      {
        os << std::setw(format[iVar]) << std::right
           << Proxy<Type>(str_data_[iVar].at(irow));
      }
      catch(...) // if an error occur, we put NA value
      { os << std::setw(format[iVar]) << std::right << stringNa;}
      os <<  delimiters_.at(0);
    }
    try
    {
      os << std::setw(format[right]) << std::right
         << Proxy<Type>(str_data_[right].at(irow));
    }
    catch(...) // if an error occur, we put NA value
    { os << std::setw(format[right]) << std::right << stringNa;}
    os << STRING_NL;
  }
}

} // namespace STK

#endif // STK_READWRITECSV_H
