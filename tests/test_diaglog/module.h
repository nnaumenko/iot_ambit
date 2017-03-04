/*
 * Copyright (C) 2016 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef MODULE_H
#define MODULE_H

template <class T>
class ModuleBase {
  protected:
    ModuleBase() {}
  public:
    ~ModuleBase() {}
    inline static T * instance(void) {
      static T inst;
      return (&inst);
    }
  public:
    const char * PROGMEM moduleName (void) {
      return (NULL);
    }
  public:
    inline void begin(void) {
      instance()->onBegin();
    }
    void onBegin(void) {}
};

template <class T>
class ModuleTimings {
  public:
    void run(void) {
      unsigned long currentTime = millis();
      if (runPeriod && ((currentTime - lastTimeRun) < runPeriod)) return;
      T::instance()->lastTimeRun = currentTime;
      T::instance()->onRun();
    }
    void onRun(void) {}
  public:
    inline unsigned long getRunPeriod(void) {
      return (runPeriod);
    }
    inline void setRunPeriod(unsigned long newRunPeriod) {
      runPeriod = newRunPeriod;
    }
  protected:
    unsigned long runPeriod = 0;
    unsigned long lastTimeRun = 0;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template <class T>
class ModuleWebServer {
  public:
    boolean onHTTPReqStart(void) {
      return (true);
    }
    boolean onHTTPReqPath(const char * path) {
      return (false);
    }
    boolean onHTTPReqMethod(const char * method) {
      return (false);
    }
    boolean onHTTPReqURLQuery(const char * name, const char * value) {
      return (true);
    }
    boolean onHTTPReqPOSTQuery(const char * name, const char * value) {
      return (true);
    }
    boolean onRespond(Print &client) {
      return (false);
    }
    boolean onHTTPReqEnd(boolean error) {
      return (true);
    }
    const char * PROGMEM getMainPath(void) {
      return(NULL);
    }
};
#pragma GCC diagnostic pop

template <class T>
class Module :
  public ModuleBase<T>,
  public ModuleTimings<T>,
  public ModuleWebServer<T>
{};

#endif
