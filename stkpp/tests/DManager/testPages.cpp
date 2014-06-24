/*--------------------------------------------------------------------*/
/*     Copyright (C) 2003  Serge Iovleff

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place,
    Suite 330,
    Boston, MA 02111-1307
    USA
  
    Contact : S..._Dot_I..._At_stkpp_Dot_org (see copyright for ...)                                   */

/* Project : DManager
 * Contents: test program for the classes IPage and ReadWritePages
 * Author  : Serge Iovleff
*/

#include "DManager.h"

using namespace STK;

class Page1 : public IPage
{
  public:
    Page1(int const& level=1) : IPage("PAGE1", level, false)
    {
      options_.reserve(2);
      options_.push_back(Option("opt1", Option::real_, false));
      options_.push_back(Option("opt2", Option::string_, false));
    }
    // dtor
    virtual ~Page1() {}
    // copy ctor
    Page1(Page1 const& page) : IPage(page)
    { }
    /* validate the options. Check if the values are coherent. */
    virtual bool validate()
    {
      stk_cout << "validating PAGE1\n";
      return true;
    }
    /** validate the page. Check if the options are coherent. */
    inline virtual Page1* clone() const
    { return new Page1(*this); }
};

class Page2 : public IPage
{
  public:
    Page2(int const& level=1) : IPage("page2", level, false)
    {
      options_.reserve(5);
      options_.push_back(Option("p2_opt1", Option::real_, false));
      options_.push_back(Option("p2_opt2", Option::string_, false));
      options_.push_back(Option("p2_opt3", Option::lreal_, false));
      options_.push_back(Option("p2_opt4", Option::lstring_, true));
      options_.push_back(Option("p2_opt5", Option::lstring_, false));
    }
    // dtor
    virtual ~Page2() {}
    // copy ctor
    Page2(Page2 const& page) : IPage(page)
    { }
    // get options
    Real opt1() const { return options_[0].get(Real());}
    std::list<Real> const& opt3() const { return options_[2].get(std::list<Real>());}
    /* validate the options. Check if the values are coherent. */
    virtual bool validate()
    {
      stk_cout << "validating page2\n";
      return true;
    }
    /** validate the page. Check if the options are coherent. */
    inline virtual Page2* clone() const
    { return new Page2(*this); }
};

class Page3 : public IPage
{
  public:
    Page3(int const& level=1) : IPage("PaGe3", level, false)
    {
      options_.push_back(Option(_T("File name"), Option::string_, false));
      options_.push_back(Option(_T("File format"), Option::string_, true));
      options_.push_back(Option(_T("Read names"), Option::string_, true));
      options_.push_back(Option(_T("Sub page"), Option::page_, false));
      // File Format
      options_[1].setValue(String(_T("csv")));
      // Sub page
      options_[3].setPage(Page2(2));
    }
    // dtor
    virtual ~Page3() {}
    // copy ctor
    Page3(Page3 const& page) : IPage(page)
    { }
    String const& opt1() const { return options_[0].get(String());}
    String const& opt2() const { return options_[1].get(String());}
    String const& opt3() const { return options_[2].get(String());}
    /* validate the options. Check if the values are coherent. */
    virtual bool validate()
    {
      stk_cout << "validating PaGe3\n";
      return true;
    }
    /** validate the page. Check if the options are coherent. */
    inline virtual Page3* clone() const
    { return new Page3(*this); }
};

/*--------------------------------------------------------------------*/
/* main.                                                              */
int main(int argc, char *argv[])
{
  stk_cout << "\n\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ test ReadWritePage                                +\n";
  stk_cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "\n\n";

  stk_cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Reading Pages rw in file ../data/options.test +\n";

  ReadWritePages rw("./tests/data/options.test");
  Page1 test1;
  rw.addPage(test1);
  Page2 test2;
  rw.addPage(test2);

  if (!rw.read("./tests/data/options.test"))
  {
    stk_cout << "Error reading file ./tests/data/options.test\n";
    stk_cout << rw.error() << _T("\n";);
    return 1;
  }
  stk_cout << _T("\n";);
  stk_cout << "Writing Pages rw\n";
  rw.write(stk_cout);

  stk_cout << _T("\n";);
  stk_cout << "Accessing individual options\n";
  stk_cout << "rw.p_page(0)->options(1).get(String()) ="
            << rw.p_page(0)->option(1).get(String())
            << _T("\n";);

  stk_cout << "rw.p_page(1)->options().front().get(Real()) ="
            << rw.p_page(1)->options().front().get(Real())
            << _T("\n";);

  stk_cout << "rw.p_page(""page2"")->option(""p2_opt1"").get(Real()) ="
            << rw.p_page("page2")->option("p2_opt1").get(Real())
            << _T("\n";);

  // create an empty page
  stk_cout << "\n\n";
  stk_cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ creating Page3\n";
  Page3 test3;
  stk_cout << "writing Page3 \n";
  test3.write(stk_cout);
  stk_cout << "\n\n";

  stk_cout << "Adding sub-page Page1\n";
  test3.addPage(test1);
  stk_cout << "writing Page3 (with sub-page Page2 and Page1)\n";
  test3.write(stk_cout);

  stk_cout << "\n\n";
  stk_cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ creating rw2 reading ../data/options.test and adding page3\n";
  ReadWritePages rw2("../data/options.test");
  rw2.addPage(test3);
  stk_cout << "\n\n";

  stk_cout << "Writing Pages rw2\n";
  rw2.write(stk_cout);

  stk_cout << "\n\n";
  stk_cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "+ Successful completion of testing for testOptionPage+\n";
  stk_cout << "+ No errors detected.                                +\n";
  stk_cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  stk_cout << "\n\n";

  return 0;
}
