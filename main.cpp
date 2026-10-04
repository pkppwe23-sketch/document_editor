// I AM CREATING A DOCUMENT EDITOR
// FOLLOWING ALL "SOLID" PRINCIPLES

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// abstraction for document elements
// only used to render the elements present in the document
class DocumentElement
{
public:
    virtual ~DocumentElement() = default;
    virtual string render() = 0;
};

// concrete implementation for text elements;
class TextElement : public DocumentElement
{
private:
    string text;

public:
    TextElement(string text)
    {
        this->text = text;
    }
    string render() override
    {
        return text;
    }
};

// class implementation of image element
class ImageElement : public DocumentElement
{
private:
    string imagepath;

public:
    ImageElement(string imagepath)
    {
        this->imagepath = imagepath;
    }

    string render() override
    {
        return "[Image:" + imagepath + "]";
    }
};

// NewLineElement represents a line break in the document.
class NewLineElement : public DocumentElement
{
public:
    string render() override
    {
        return "\n";
    }
};

// TabSpaceElement represents a tab space in the document.
class TabSpaceElement : public DocumentElement
{
public:
    string render() override
    {
        return "\t";
    }
};

// Document class responsible for holding a collection of elements.
class Document
{
private:
    vector<DocumentElement*> documentElements;

public:
    ~Document()
    {
        for (auto element : documentElements)
        {
            delete element;
        }
    }

    void addElement(DocumentElement* element)
    {
        documentElements.push_back(element);
    }

    // Renders the document by concatenating the rendered elements.
    string render()
    {
        string result;

        for (auto element : documentElements)
        {
            result += element->render();
        }

        return result;
    }
};

// Persistence Interface
class Persistence
{
public:
    virtual ~Persistence() = default;
    virtual void save(string data) = 0;
};

// FileStorage implementation of Persistence
class FileStorage : public Persistence
{
public:
    void save(string data) override
    {
        ofstream outFile("document.txt");
        if (outFile)
        {
            outFile << data;
            outFile.close();
            cout << "Document saved to document.txt" << endl;
        }
        else
        {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

// Placeholder DBStorage implementation
class DBStorage : public Persistence
{
public:
    void save(string data) override
    {
        // Save to DB
    }
};

// DocumentEditor class managing client interactions
class DocumentEditor
{
private:
    Document* document;
    Persistence* storage;
    string renderedDocument;

public:
    DocumentEditor(Document* document, Persistence* storage)
    {
        this->document = document;
        this->storage = storage;
    }

    void addText(string text)
    {
        document->addElement(new TextElement(text));
    }

    void addImage(string imagePath)
    {
        document->addElement(new ImageElement(imagePath));
    }

    // Adds a new line to the document.
    void addNewLine()
    {
        document->addElement(new NewLineElement());
    }

    // Adds a tab space to the document.
    void addTabSpace()
    {
        document->addElement(new TabSpaceElement());
    }

    string renderDocument()
    {
        if (renderedDocument.empty())
        {
            renderedDocument = document->render();
        }
        return renderedDocument;
    }

    void saveDocument()
    {
        storage->save(renderDocument());
    }
};

// Client usage example
int main()
{
    Document* document = new Document();
    Persistence* persistence = new FileStorage();

    DocumentEditor* editor = new DocumentEditor(document, persistence);

    // Simulate a client using the editor with common text formatting features.
    editor->addText("Hello, world!");
    editor->addNewLine();
    editor->addText("This is a real-world document editor example.");
    editor->addNewLine();
    editor->addTabSpace();
    editor->addText("Indented text after a tab space.");
    editor->addNewLine();
    editor->addImage("picture.jpg");

    // Render and display the final document.
    cout << editor->renderDocument() << endl;

    editor->saveDocument();

    delete editor;
    delete persistence;
    delete document;

    return 0;
}
