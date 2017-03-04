/*
 * Copyright (C) 2016-2017 Nick Naumenko (https://github.com/nnaumenko)
 * All rights reserved
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

/**
 * @file
 * @brief Defines interfaces for software modules.
 * @details Since module usage is fully defined at compile time,
 * modules do not use virtual methods and rely on Curiously
 * Recurring Template Pattern (CRTP) instead.
 * @details Each software module is a singleton.
 */

#ifndef MODULE_H
#define MODULE_H

/// @brief Basic interface for a software module.
/// @detials Implements singleton, Module Name C-String and module initialisation.
/// @tparam T CRTP template parameter (derived class)
template <class T>
class ModuleBase {
  protected:
    ModuleBase() {}
  public:
    ~ModuleBase() {}
    inline static T * instance(void) {
      /// @brief Implements singleton pattern.
      /// @return Actual module instance to work with.
      static T inst;
      return (&inst);
    }
  public:
    const char * PROGMEM moduleName (void) {
      /// @brief To be implemented by actual module if this functionality is required.
      ///
      /// @return C-string in PROGMEM representing human-readable module name or NULL.
      /// if no module name is specified.
      return (NULL);
    }
  public:
    inline void begin(void) {
      /// Call this method from from setup() function to initialise the module.
      instance()->onBegin();
    }
    void onBegin(void) {
      /// @brief To be implemented by actual module if this functionality is required.
      ///
      /// In this method the module initialisation is performed.
    }
};

/// @brief Interface for the software module which needs to be automatically executed
/// on a periodic basis.
/// @details After onRun() method is called, the current millis() value is monitored,
/// and when the specified period of time passes, onRun() is called again.
/// @warning Since onRun() method is called sequentially for all modules and
/// method execution time is not controlled, the time span before next onRun() call
/// is NOT guranteed to be exactly as specified. It is only guranteed that onRun()
/// will be called no sooner than the specified time span.
/// @tparam T CRTP template parameter (derived class)
template <class T>
class ModuleTimings {
  public:
    void run(void) {
      /// Call this method from loop() function to make module perform its function
      uint32_t currentTime = millis();
      if (runPeriod && ((currentTime - lastTimeRun) < runPeriod)) return;
      T::instance()->lastTimeRun = currentTime;
      T::instance()->onRun();
    }
    void onRun(void) {
      /// @brief To be implemented by actual module if this functionality is required.
      ///
      /// In this method the module main operation is performed.
    }
  public:
    inline uint32_t getRunPeriod(void) const {
      /// Returns time period to cyclically execute onRun method.
      /// @return Time (in milliseconds) between cyclic onRun method executions.
      return (runPeriod);
    }
    inline void setRunPeriod(uint32_t newRunPeriod) {
      /// Sets time period to cyclically execute onRun method.
      /// @param newRunPeriod Time (in milliseconds) between cyclic onRun method executions.
      runPeriod = newRunPeriod;
    }
  protected:
    uint32_t runPeriod = 0; ///< Time period in milliseconds to cyclically execute onRun method
    uint32_t lastTimeRun = 0; ///< Last time in milliseconds when the onRun method was called
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
/// @brief Interface for the software module which integrates with webserver
/// @details The methods belonging to this interface are called as follows:
/// @details
/// * When HTTP request is received, onHTTPReqStart() is called. The module
/// performs any initialisation required to take part in request processing.
/// * HTTP request is being parsed and onHTTPReqPath() is called. The module
/// decides whether it can produce the response for this path and returns true
/// if yes. If onHTTPReqPath() returns false, onHTTPReqMethod(), onHTTPReqURLQuery(),
/// onHTTPReqPOSTQuery() and onRespond() below are not called.
/// * onHTTPReqMethod() is called. The module decides whether it can produce
/// the response for this method combined with the path specified earlier and
/// returns true if yes. If onHTTPReqMethod() returns false, onHTTPReqURLQuery(),
/// onHTTPReqPOSTQuery() and onRespond() below are not called.
/// * If HTTP request URL contains Query String, onHTTPReqURLQuery() is called
/// for each name=value pair. The module records the parameters to produce
/// requested response.
/// * If HTTP request URL contains POST Query String, onHTTPReqPOSTQuery() is
/// called for each name=value pair. The module records the parameters to produce
/// requested response. Note: HTTP request may contain both URL Query String and
/// POST Query String.
/// * When HTTP request parsing is finished, onRespond() is called. The module
/// produces the HTTP-response complete with Status Line, Header Fields and Message
/// Body.
/// * onHTTPReqEnd() is called. The module performs any cleanup required to restore
/// to its normal state and to be ready for the onHTTPReqStart() call when the next
/// HTTP request is received.
/// @tparam T CRTP template parameter (derived class)
template <class T>
class ModuleWebServer {
  public:
    boolean onHTTPReqStart(void) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Called when HTTP request is received and is about to be processed.
      /// This method is always called regardless of the HTTP request content.
      /// @details In this method module initialises its webserver-related functionality.
      /// @return In current implementation return value is ignored. To maintain
      /// compatibility with future versions this method should always return true.
      return (true);
    }
    boolean onHTTPReqPath(const char * path) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Called after onHTTP ReqStart() when path is extracted from HTTP
      /// request.
      /// @details In this method module checks the path specified in HTTP request and
      /// makes a decision whether it accepts this path and is able to produce a response.
      /// @param path Path from HTTP request. The path is already URL-decoded
      /// before it is passed to the module.
      /// @return If the module accepts specified path and is able to produce a response,
      /// this method returns true. If module does not accept specified path and is
      /// unable to produce a response, this method returns false.
      return (false);
    }
    boolean onHTTPReqMethod(const char * method) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Called after onHTTPReqPath and only if previous call of onHTTPReqPath()
      /// returned true.
      /// @details In this method module checks the method specified in HTTP request and
      /// makes a decision whether it can accept this method and produce a response based
      /// on path and method.
      /// @param method Method from HTTP request.
      /// @return If the module accepts specified method in combination with previously
      /// specified path and is able to produce a response, this method returns true.
      /// Otherwise this method returns false.
      return (false);
    }
    boolean onHTTPReqURLQuery(const char * name, const char * value) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Can be called once or multiple times after onHTTPReqMethod() and only
      /// if previous calls of onHTTPReqPath() and onHTTPReqMethod() both returned true.
      /// @details Only called if HTTP request URL contains Query String.
      /// @details In this method module checks the name and value of the Query String
      /// item and sets module internal values to produce the correct response.
      /// @param name Name(key) of the Query String item.
      /// @param value Value of the Query String item.
      /// @return In current implementation return value is ignored. To maintain
      /// compatibility with future versions this method should always return true.
      return (true);
    }
    boolean onHTTPReqPOSTQuery(const char * name, const char * value) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Can be called once or multiple times after onHTTPReqMethod() and only
      /// if previous calls of onHTTPReqPath() and onHTTPReqMethod() both returned true.
      /// @details Only called if HTTP request contains POST Query String.
      /// @details In this method module checks the name and value of the Query String
      /// item and sets module internal values to produce the correct response.
      /// @param name Name(key) of the POST Query String item.
      /// @param value Value of the POST Query String item.
      /// @return In current implementation return value is ignored. To maintain
      /// compatibility with future versions this method should always return true.
      return (true);
    }
    boolean onRespond(Print &client) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Called when HTTP request processing is completed and the response
      /// is to be produced.
      /// @details Only called if previous calls of onHTTPReqPath() and onHTTPReqMethod()
      /// both returned true.
      /// @details In this method module generates a HTTP response, complete with Status
      /// Line, Header Fields (if any) and Message Body (HTML, JSON, XML, plain text, etc.).
      /// @details No part of the response is produced by the caller routine.
      /// @details The response is generated by calling print() / println() methods of the
      /// client parameter.
      /// @param client Print class to send the response to.
      /// @return If no error occured during request generation, this method returns
      /// true; otherwise it returns false.
      return (false);
    }
    boolean onHTTPReqEnd(boolean error) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @details Called when HTTP request processing is completed and the response
      /// was generated.
      /// @details This method is always called regardless of the HTTP request content
      /// and previous return values.
      /// @details In this method module performs actions after HTTP request was
      /// processed.
      /// @details The module must assume that onHTTPReqStart() method will only be
      /// called when next HTTP request is received by webserver (which might never
      /// happen as well), so if any actions must be taken to restore the module to
      /// normal state, they are placed in this method.
      /// @param error Set to true if there was an error parsing HTTP request.
      /// @return In current implementation return value is ignored. To maintain
      /// compatibility with future versions this method should always return true.
      return (true);
    }
    const char * PROGMEM getMainPath(void) {
      /// @brief To be implemented by actual module if this functionality is required.
      /// @return C-string in PROGMEM representing default webserver path (relative
      /// to root) for this module (e.g. "/mymodulepath" or "/"). Returns NULL if no
      /// default module path is specified, e.g. if module does not integrate with
      /// webserver.
      return (NULL);
    }
};
#pragma GCC diagnostic pop


/// @brief Base class for a software module, combines all module interfaces.
/// Software modules are inherited from this class.
/// @tparam T CRTP template parameter

template <class T>
class Module :
  public ModuleBase<T>,
  public ModuleTimings<T>,
  public ModuleWebServer<T>
{};

#endif
