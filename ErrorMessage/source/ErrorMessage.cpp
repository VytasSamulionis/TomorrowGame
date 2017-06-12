#include "../include/ErrorMessage.h"
#include <cstring>
#include <cstdio>

ErrorMessage::ErrorMessage(ERROR_CODE _errorCode, const char* _filename, UINT _line, const char* _details) {
    m_ErrorCode = _errorCode;
    if (_filename) {
        strcpy (m_Filename, _filename);
    } else {
        m_Filename[0] = '\0';
    }
    m_Line = _line;
    if (_details) {
        strcpy (m_Details, _details);
    } else {
        m_Details[0] = '\0';
    }
}

const char* ErrorMessage::GetErrorMessage () {
    memset(m_Message, 0, MAX_MSG_LENGTH);
    switch (m_ErrorCode) {
        case ERRC_OUT_OF_MEM:
            if (strlen (ERRCDESC_OUT_OF_MEM) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_OUT_OF_MEM);
            }
            break;
        case ERRC_API_CALL:
            if (strlen (ERRCDESC_API_CALL) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_API_CALL);
            }
            break;
        case ERRC_FILE_NOT_FOUND:
            if (strlen (ERRCDESC_FILE_NOT_FOUND) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_FILE_NOT_FOUND);
            }
            break;
        case ERRC_BAD_FILE:
            if (strlen (ERRCDESC_BAD_FILE) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_BAD_FILE);
            }
            break;
        case ERRC_OUT_OF_RANGE:
            if (strlen (ERRCDESC_OUT_OF_RANGE) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_OUT_OF_RANGE);
            }
            break;
        case ERRC_NO_DEVICE:
            if (strlen (ERRCDESC_NO_DEVICE) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_NO_DEVICE);
            }
            break;
        case ERRC_UNKNOWN_VF:
            if (strlen (ERRCDESC_UNKNOWN_VF) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_UNKNOWN_VF);
            }
            break;
        case ERRC_INVALID_PARAMETER:
            if (strlen (ERRCDESC_INVALID_PARAMETER) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_INVALID_PARAMETER);
            }
            break;
        case ERRC_NOT_READY:
            if (strlen (ERRCDESC_NOT_READY) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_NOT_READY);
            }
            break;
        case ERRC_UNDEFINED:
            // go to default
        default:
            if (strlen (ERRCDESC_UNDEFINED) < MAX_MSG_LENGTH) {
                strcpy (m_Message, ERRCDESC_UNDEFINED);
            }
            break;
    }
    m_Message[strlen (m_Message)] = '\n';
    if (m_Filename && strlen (m_Message) + strlen (m_Filename) < MAX_MSG_LENGTH) {
        strcat (m_Message, "File: ");
        strcat (m_Message, m_Filename);
    }
    sprintf (m_Message, "%s\nLine: %u\n", m_Message, m_Line);
    if (m_Details && strlen (m_Message) + strlen (m_Details) < MAX_MSG_LENGTH) {
        strcat (m_Message, m_Details);
    }
    return m_Message;
}

void ErrorMessage::SetErrorCode (ERROR_CODE _errorCode) {
    m_ErrorCode = _errorCode;
}

ERROR_CODE ErrorMessage::GetErrorCode () const {
    return m_ErrorCode;
}

void ErrorMessage::SetFilename (const char* _filename) {
    strcpy (m_Filename, _filename);
}

const char* ErrorMessage::GetFilename () const {
    return m_Filename;
}

void ErrorMessage::SetLine (UINT _line) {
    m_Line = _line;
}

UINT ErrorMessage::GetLine () const {
    return m_Line;
}

void ErrorMessage::SetDetails (const char* _details) {
    strcpy (m_Details, _details);
}

const char* ErrorMessage::GetDetails () const {
    return m_Details;
}