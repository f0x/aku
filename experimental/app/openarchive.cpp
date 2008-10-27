#include "openarchive.h"

#include <KFileItem>
#include <KDebug>

OpenArchive::OpenArchive()
{
}

OpenArchive::~OpenArchive()
{
}

void OpenArchive::load(KUrl url)
{
  if (!KFileItem(KFileItem::Unknown, KFileItem::Unknown, url).isReadable()) return;
  mimetype = KMimeType::findByUrl(url);
  kDebug() << mimetype -> name();
}

void OpenArchive::buildTree()
{
}
