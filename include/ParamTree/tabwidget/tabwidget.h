#ifndef TabWidget_H
#define TabWidget_H

#include <QWidget>
#include <QHash>
#include <QStackedLayout>
#include <QTabBar>
#include <QTabWidget>
#include <QFrame>
#include <QStringList>

class TabPage;

class StackedLayout : public QStackedLayout
{
public:
    StackedLayout();
    virtual QSize maximumSize() const override;
};

class FramedPage : public QFrame
{
public:
    FramedPage(TabPage* tab_page, QWidget* parent = nullptr);
    TabPage* tabPage() {return m_page;}
private:
    TabPage* m_page;
};

class StackedWidget : public QWidget
{
    Q_OBJECT
public:
    StackedWidget(QWidget* parent= nullptr);
    void addPage(const QStringList& key,TabPage* widget);
    void removePage(const QStringList& key);
    void setCurrentPage(int index);
    bool contains(const QStringList& key) const;
    int count() const {return m_pages.count();}
    int indexOf(const QStringList& key) const;
    TabPage* currentPage() const;
    TabPage* tabPage(const QStringList& key) const;
private:
    QHash<QStringList,FramedPage*> m_pages;
    StackedLayout* m_stacked_layout;
};

class TabWidget : public QWidget
{
    Q_OBJECT

public:
    TabWidget(TabPage* parent = nullptr);
    void addTab(const QStringList& key,TabPage* tab_page);
    void removeTab(const QStringList& key);
    bool contains(const QStringList& key) const;
    int count() const;
//    TabPage* page(int index) const;
    TabPage* tabPage(const QStringList& key) const;
    TabPage* currentPage() const;

public slots:
    void tab_changed(int index);

private:
    TabPage* m_parent;
    StackedWidget* m_stacked_widget;
    QTabBar* m_tab_bar;
};

#endif // TabWidget_H
