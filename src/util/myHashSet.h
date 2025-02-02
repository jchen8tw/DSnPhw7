/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet {
   public:
    HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) {
        if (b != 0) init(b);
    }
    ~HashSet() { reset(); }

    // TODO: implement the HashSet<Data>::iterator
    // o An iterator should be able to go through all the valid Data
    //   in the Hash
    // o Functions to be implemented:
    //   - constructor(s), destructor
    //   - operator '*': return the HashNode
    //   - ++/--iterator, iterator++/--
    //   - operators '=', '==', !="
    //
    class iterator {
        friend class HashSet<Data>;

       public:
        iterator(vector<Data>* b, size_t nb, size_t cur = 0,size_t index = 0)
            : _numBuckets(nb), _buckets(b), _curbucket(cur), _index(index) {}
        const Data& operator*() const { return _buckets[_curbucket][_index]; }
        iterator& operator++() {
            _index++;
            while (_curbucket <= _numBuckets-1 && _index >= _buckets[_curbucket].size()) {
                _index = 0;
                _curbucket++;
            }
            return (*this);
        }
        bool operator!=(const iterator& i) const {
            if (_curbucket != i._curbucket || i._index != _index)
                return true;
            else
                return false;
        }
        void setData(size_t i, size_t curb) {
            _index = i;
            _curbucket = curb;
        }

       private:
        size_t _curbucket;
        size_t _index;
        size_t _numBuckets;
        vector<Data>* _buckets;
    };

    void init(size_t b) {
        _numBuckets = b;
        _buckets = new vector<Data>[b];
    }
    void reset() {
        _numBuckets = 0;
        if (_buckets) {
            delete[] _buckets;
            _buckets = 0;
        }
    }
    void clear() {
        for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
    }
    size_t numBuckets() const { return _numBuckets; }

    vector<Data>& operator[](size_t i) { return _buckets[i]; }
    const vector<Data>& operator[](size_t i) const { return _buckets[i]; }

    // TODO: implement these functions
    //
    // Point to the first valid data
    iterator begin() const {
        if (empty())
            return end();
        else {
            iterator biterator(_buckets, _numBuckets, 0);
            for (vector<Data>* b = _buckets; b < _buckets + _numBuckets; b++) {
                if (!b->empty()) {
                    biterator.setData(0, b - _buckets);
                    break;
                }
            }
            return biterator;
        }
    }
    // Pass the end
    iterator end() const {
        return iterator(_buckets, _numBuckets, _numBuckets,0);
    }
    // return true if no valid data
    bool empty() const {
        if (_buckets == 0)
            return true;
        else {
            bool isempty = true;
            for (size_t i = 0; i < _numBuckets; i++) {
                if (!_buckets[i].empty()) {
                    isempty = false;
                    break;
                }
            }
            return isempty;
        }
    }
    // number of valid data
    size_t size() const {
        size_t s = 0;
        for (size_t i = 0; i < _numBuckets; i++) {
            s += _buckets[i].size();
        }
        return s;
    }

    // check if d is in the hash...
    // if yes, return true;
    // else return false;
    bool check(const Data& d) const {
        size_t b = bucketNum(d);
        for (int i = 0; i < _buckets[b].size(); i++) {
            if (_buckets[b][i] == d) return true;
        }
        return false;
    }

    // query if d is in the hash...
    // if yes, replace d with the data in the hash and return true;
    // else return false;
    bool query(Data& d) const {
        size_t b = bucketNum(d);
        for (size_t i = 0; i < _buckets[b].size(); i++) {
            if (_buckets[b][i] == d) {
                _buckets[b][i] = d;
                return true;
            }
        }
        return false;
    }

    // update the entry in hash that is equal to d (i.e. == return true)
    // if found, update that entry with d and return true;
    // else insert d into hash as a new entry and return false;
    bool update(const Data& d) {
        size_t b = bucketNum(d);
        for (size_t i = 0; i < _buckets[b].size(); i++) {
            if (_buckets[b][i] == d) {
                _buckets[b][i] = d;
                return true;
            }
        }
        _buckets[b].push_back(d);
        return false;
    }

    // return true if inserted successfully (i.e. d is not in the hash)
    // return false is d is already in the hash ==> will not insert
    bool insert(const Data& d) {
        size_t b = bucketNum(d);
        for (size_t i = 0; i < _buckets[b].size(); i++) {
            if (_buckets[b][i] == d) {
                return false;
            }
        }
        _buckets[b].push_back(d);
        return true;
    }

    // return true if removed successfully (i.e. d is in the hash)
    // return fasle otherwise (i.e. nothing is removed)
    bool remove(const Data& d) { 
       size_t b = bucketNum(d);
       for(size_t i = 0;i<_buckets[b].size();i++){
          if(_buckets[b][i] == d){
             //_buckets[b].erase(_buckets[b].cbegin()+i);
             swap(_buckets[b][i],_buckets[b].back());
             _buckets[b].resize(_buckets[b].size()-1);
             return true;
          }
       }
       return false;
    }

   private:
    // Do not add any extra data member
    size_t _numBuckets;
    vector<Data>* _buckets;

    size_t bucketNum(const Data& d) const { return (d() % _numBuckets); }
};

#endif  // MY_HASH_SET_H
