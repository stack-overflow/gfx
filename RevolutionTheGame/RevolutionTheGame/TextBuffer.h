#pragma once
#include <fstream>

#pragma warning(push)
// warning C4996: 'strncpy': This function or variable may be unsafe. Consider using strncpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable: 4996)

class TextBuffer
{
private:
    const char *m_filename;
    char *m_textBuffer;
    size_t m_fLength;

private:
    int ReleaseTextBuffer()
    {
		if (m_textBuffer)
		{
            delete [] m_textBuffer;
            m_textBuffer = NULL;
        }

        return 0;
    }

    int LoadTextFile(const char *_filename)
    {
        std::ifstream inStream(_filename, std::ios::binary);

        inStream.seekg(0, std::ios::end);
        m_fLength = static_cast<size_t>(inStream.tellg());
        inStream.seekg(0, std::ios::beg);

        if(m_fLength == 0)
        {
            return -1;
        }

        ReleaseTextBuffer();

        m_textBuffer = new char[m_fLength];

        if(m_textBuffer == NULL)
        {
            return -2;
        }

        inStream.read(m_textBuffer, m_fLength);

        inStream.close();

        return m_fLength;
    }

    size_t LoadTextFromBuffer(char *_buffer, size_t _len)
    {
        if(_buffer)
        {
            m_fLength = _len;

            ReleaseTextBuffer();

            m_textBuffer = new char[m_fLength + 1];

            strncpy(m_textBuffer, _buffer, m_fLength + 1);

            return m_fLength;
        }

        return -1;
    }

public:
    TextBuffer() :
        m_filename(NULL),
        m_textBuffer(NULL),
        m_fLength(0)
    {}

    TextBuffer(const char *_filename) :
		m_filename(_filename),
		m_textBuffer(NULL),
        m_fLength(0)
    {
        LoadTextFile(_filename);
    }

    TextBuffer(char *_buffer, size_t _len) :
        m_filename(NULL),
		m_textBuffer(NULL),
        m_fLength(0)
    {
        LoadTextFromBuffer(_buffer, _len);
    }

    TextBuffer(TextBuffer& _otherTextBuffer)
    {
        strncpy(_otherTextBuffer.m_textBuffer, m_textBuffer, m_fLength + 1);
    }

    ~TextBuffer() {
        ReleaseTextBuffer();
    }

    int LoadFromFile(const char *_filename)
    {
        ReleaseTextBuffer();
        m_filename = _filename;
        return LoadTextFile(m_filename);
    }

    int LoadFromBuffer(char *_buffer, size_t _len)
    {
        ReleaseTextBuffer();
        m_filename = NULL;
        return LoadTextFromBuffer(_buffer, _len);
    }

    int ReloadFromFile()
    {
        ReleaseTextBuffer();
        return LoadTextFile(m_filename);
    }

    char *&GetText()
    {
        return m_textBuffer;
    }

    size_t GetLength()
    {
        return m_fLength;
    }

    char *&operator()()
    {
        return m_textBuffer;
    }
};

#pragma warning(pop)
