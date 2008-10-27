#ifndef OPENARCHIVE_H
#define OPENARCHIVE_H

#include <KUrl>
#include <KMimeType>

class OpenArchive : public QObject
{
  Q_OBJECT

  public:
    OpenArchive();
    ~OpenArchive();

    void load (KUrl url);

  private:
    KMimeType::Ptr mimetype;
   
    void buildTree();

};

#endif
