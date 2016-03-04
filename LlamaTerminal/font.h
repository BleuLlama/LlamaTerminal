#ifndef FONT_H
#define FONT_H

#include <QObject>

typedef struct lfont {
    int startchar;
    int maxchar;
    int w;
    int h;
    const unsigned char * data;
} LFONT;

extern LFONT internalFont;

////////////////////////////////////////////////////////
class Font : public QObject
{
    Q_OBJECT
public:
    explicit Font(QObject *parent = 0);

signals:

public slots:
};

#endif // FONT_H
