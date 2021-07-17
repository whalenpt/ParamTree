#include "tabwidget/tabwidget.h"
#include "tabwidget/tabpage.h"

#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QDebug>

namespace paramtree{

StackedLayout::StackedLayout() : QStackedLayout()
{
    setSizeConstraint(QLayout::SetMaximumSize);
}

QSize StackedLayout::maximumSize() const
{
    QSize base_size(QStackedLayout::maximumSize());
    return QSize(base_size.width(),currentWidget()->sizeHint().height());
}

FramedPage::FramedPage(TabPage* tab_page, QWidget* parent) :
    QFrame(parent),
    m_page(tab_page)
{
    setFrameStyle(QFrame::Sunken|QFrame::StyledPanel);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QVBoxLayout* vbox = new QVBoxLayout;
    vbox->addWidget(tab_page);
    setLayout(vbox);
}

StackedWidget::StackedWidget(QWidget* parent)  : QWidget(parent),
   m_stacked_layout(new StackedLayout)
{
    setLayout(m_stacked_layout);
}

void StackedWidget::addPage(const QStringList& key,TabPage* tab_page){
    FramedPage* framed_page = new FramedPage(tab_page);
    m_pages.insert(key,framed_page);
    m_stacked_layout->addWidget(framed_page);
}

void StackedWidget::removePage(const QStringList &key)
{
    FramedPage* framed_page = m_pages.take(key);
    m_stacked_layout->removeWidget(framed_page);
    delete framed_page;
}

bool StackedWidget::contains(const QStringList& key) const
{
    return m_pages.contains(key);
}

int StackedWidget::indexOf(const QStringList& key) const
{
    FramedPage* framed_page = m_pages.value(key);
    if(framed_page)
        return m_stacked_layout->indexOf(framed_page);
    return -1;
}

void StackedWidget::setCurrentPage(int index)
{
    m_stacked_layout->setCurrentIndex(index);
}

TabPage* StackedWidget::tabPage(const QStringList& key) const
{
    if(m_pages.contains(key)){
        FramedPage* framed_page = m_pages.value(key);
        return framed_page->tabPage();
    }
    // This bit of code is buggy!!!!!!
    QHashIterator<QStringList,FramedPage*> iter(m_pages);
    while(iter.hasNext()){
        iter.next();
        TabPage* tab_page = iter.value()->tabPage()->tabPage(key);
        if(tab_page != nullptr)
            return tab_page;
    }
    return nullptr;
}

TabPage* StackedWidget::currentPage() const
{
    FramedPage* framed_page = dynamic_cast<FramedPage*>(m_stacked_layout->currentWidget());
    return framed_page->tabPage();
}

TabWidget::TabWidget(TabPage* parent) : QWidget(parent),
    m_parent(parent),
    m_stacked_widget(new StackedWidget),
    m_tab_bar(new QTabBar)
{
    connect(m_tab_bar,&QTabBar::currentChanged,this,
            &TabWidget::tab_changed);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_tab_bar);
    layout->addWidget(m_stacked_widget);
    setLayout(layout);
}

void TabWidget::tab_changed(int index)
{
    m_stacked_widget->setCurrentPage(index);
}

void TabWidget::addTab(const QStringList& key,TabPage* tab_page)
{
    m_tab_bar->addTab(key.last());
    m_stacked_widget->addPage(key,tab_page);
}

void TabWidget::removeTab(const QStringList& key)
{
    m_tab_bar->removeTab(m_stacked_widget->indexOf(key));
    m_stacked_widget->removePage(key);
}

bool TabWidget::contains(const QStringList& key) const
{
    return m_stacked_widget->contains(key);
}

int TabWidget::count() const
{
    return m_stacked_widget->count();
}

TabPage* TabWidget::currentPage() const
{
    return m_stacked_widget->currentPage();
}

TabPage* TabWidget::tabPage(const QStringList& key) const
{
    return m_stacked_widget->tabPage(key);
}

}









