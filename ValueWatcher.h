#ifndef VALUEWATCHER_H_
#define VALUEWATCHER_H_

template <typename T>
class ValueWatcher
{
protected:
  T prev;
  T current;
  bool changed;
  void (*_onValueChangedListener)(ValueWatcher<T> *) = 0;
  /* data */
  int id = -1;

public:
  ValueWatcher(/* args */) {}
  ~ValueWatcher() {}

  void start(T value)
  {
    prev = value;
    current = value;
  }
  bool update(T value)
  {
    prev = current;
    current = value;

    bool c = changed = (prev != current);

    if (changed && _onValueChangedListener != 0)
    {
      _onValueChangedListener(this);
    }
    return c;
  }

  T getValue()
  {
    return current;
  }
  T getPrev()
  {
    return prev;
  }

  void setOnValueChangedListener(void (*onValueChangedListener)(ValueWatcher<T> *))
  {
    _onValueChangedListener = onValueChangedListener;
  }
  void reset()
  {
    current = 0;
     prev = 0;
    changed = false;
  }

  void consume()
  {
    changed = false;
  }

  bool hasChanged()
  {
    return changed;
  }

  bool isBigger()
  {
    return current > prev;
  }

  bool isSmaller()
  {
    return current < prev;
  }

  void setID(int id)
  {
    this->id = id;
  }

  int getID()
  {
    return id;
  }
};

#endif