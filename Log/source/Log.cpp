/*  Author: Vytas Samulionis
    Description: A class for creating a log file.
*/
#include "../include/Log.h"

LogManager::LogManager (const char* _fileName, bool _autoFlush) {    
    m_Log = fopen (_fileName, "w");
    if (!m_Log) {
        m_IsOpened = false;
    }
    m_IsOpened = true;
    m_IsAutoFlushing = _autoFlush;
}

LogManager::~LogManager () {
    if (m_IsOpened) {
        fclose (m_Log);
        m_Log = NULL;
        m_IsOpened = false;
    }    
}

void LogManager::Log (char* _format, ...) {
    if (!m_IsOpened) {
        return;
    }
    char buffer[256];
    va_list args;
    va_start (args, _format);
    vsprintf (buffer, _format, args);
    fprintf (m_Log, buffer);
    if (m_IsAutoFlushing) {
        fflush (m_Log);
    }
}

bool LogManager::IsRunning () const {
    return m_IsOpened;
}

bool LogManager::IsAutoFlushing () const {
    return m_IsAutoFlushing;
}

void LogManager::SetAutoFlushing (bool _autoFlush) {
    m_IsAutoFlushing = _autoFlush;
}