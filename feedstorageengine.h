#ifndef FEEDSTORAGEENGINE_H
#define FEEDSTORAGEENGINE_H

#include "feed.h"

class FeedStorageEngine
{
public:
    virtual int addFeed(Feed* feed) = 0;
};

#endif // FEEDSTORAGEENGINE_H
