// Document/View sample for Boost.Signals2.
// Expands on doc_view_acm.cpp example by using boist::signals2::deconstruct
// as a post-constructing factory function.
//
// Copyright Keith MacDonald 2005.
// Copyright Frank Mori Hess 2009.
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// For more information, see http://www.boost.org

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost_signals2/deconstruct.hpp>
#include <boost_signals2/signal.hpp>

class Document
{
public:
  typedef boist::signals2::signal<void ()>  signal_t;

public:
  Document()
  {}

  /* Connect a slot to the signal which will be emitted whenever
    text is appended to the document. */
  boist::signals2::connection connect(const signal_t::slot_type &subscriber)
  {
    return m_sig.connect(subscriber);
  }

  void append(const char* s)
  {
    m_text += s;
    m_sig();
  }

  const std::string& getText() const
  {
    return m_text;
  }

private:
  signal_t    m_sig;
  std::string m_text;
};

class TextView
{
public:
  /* This adl_postconstruct function will be found
  via argument-dependent lookup when using boist::signals2::deconstruct. */
  template<typename T>
    friend void adl_postconstruct(const std::shared_ptr<T> &view_sp, TextView *view, Document& doc)
  {
    view->m_document = &doc;
    {
      typedef Document::signal_t::slot_type slot_type;
      slot_type myslot(&TextView::refresh, view);
      doc.connect(myslot.track(view_sp));
    }
  }

  void refresh() const
  {
    std::cout << "TextView: " << m_document->getText() << std::endl;
  }
private:
  // give boist::signals2::deconstruct access to private constructor
  friend class boist::signals2::deconstruct_access;
  // private constructor to force use of deconstruct
  TextView() : m_document(0)
  {}

  Document*               m_document;
};

class HexView
{
public:
  /* This adl_postconstruct function will be found
  via argument-dependent lookup when using boist::signals2::deconstruct. */
  template<typename T>
    friend void adl_postconstruct(const std::shared_ptr<T> &view_sp, HexView *view, Document& doc)
  {
    view->m_document = &doc;
    {
      typedef Document::signal_t::slot_type slot_type;
      slot_type myslot(&HexView::refresh, view);
      doc.connect(myslot.track(view_sp));
    }
  }

  void refresh() const
  {
    const std::string&  s = m_document->getText();

    std::cout << "HexView:";

    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
      std::cout << ' ' << std::hex << static_cast<int>(*it);

    std::cout << std::endl;
  }
private:
  // give boist::signals2::deconstruct access to private constructor
  friend class boist::signals2::deconstruct_access;
  // private constructor to force use of deconstruct
  HexView(): m_document(0)
  {}

  Document*               m_document;
};

namespace bs2 = boist::signals2;

int main(int argc, char* argv[])
{
  Document    doc;
  std::shared_ptr<TextView> v1 = bs2::deconstruct<TextView>().postconstruct( std::ref(doc));
  std::shared_ptr<HexView> v2 = bs2::deconstruct<HexView>().postconstruct( std::ref(doc));

  doc.append(argc >= 2 ? argv[1] : "Hello world!");

  v2.reset(); // destroy the HexView, automatically disconnecting
  doc.append("  HexView should no longer be connected.");

  return 0;
}
