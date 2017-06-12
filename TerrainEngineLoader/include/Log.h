/** @file Log.h
Log Manager.
Controls the simple logging.
*/
#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <cstdio>
#include <cstdarg>

/** Controls the logging.
Opens simple text file and controls writing formated C style text to it. */
class LogManager {
public:
    /** Constructor.
    @param[in] _filename the name of the log file
    @param[in] _autoFlush is auto flushing information to the file enabled.
    Default value is @c true */
    LogManager (const char* _filename, bool _autoFlush = true);

    /** Destuctor. */
    ~LogManager  ();

    /** Writes formated message to the log. 
    @param[in] _format C style format of a message

    The other arguments are arguments for the formated message. */
    void Log (char* _format, ...);

    /** Returns status.
    @return @c true log manager is running and ready
    @return @c false log manager is not prepared */
    bool IsRunning () const;

    /** Shows if data is flushed after each call
    @return @c true data is flushed after each call
    @return @c false data is flushed before the file is closed */
    bool IsAutoFlushing () const;
    
    /** Setter: auto flush
    @param[in] _autoFlush enable auto flushing */
    void SetAutoFlushing (bool _autoFlush);

private:
    FILE* m_Log;            /**< Log file */
    bool m_IsOpened;        /**< Is file opened? */
    bool m_IsAutoFlushing;  /**< Is the data flushed after each call? */
};

#endif