//
// Created by Gresstant on 2020/2/13.
//

#ifndef OSAFORK_LISTITERATEHELPER_H
#define OSAFORK_LISTITERATEHELPER_H

template <typename T>
class ListIterateHelper {
public:
    virtual void removeFirst() = 0;
    virtual void removeLast() = 0;
    virtual void removePreviousIfExists() = 0;
    virtual void removeThis() = 0;
    virtual void removeNextIfExists() = 0;
    virtual void insertAsFirst(T* item) = 0;
    virtual void insertAsLast(T* item) = 0;
    virtual void insertAsPrevious(T* item) = 0;
    virtual void insertAsNext(T* item) = 0;
};

#endif //OSAFORK_LISTITERATEHELPER_H
