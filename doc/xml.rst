===============
 XML Utilities
===============
The ``sup/xml`` package provides tools for working with XML data, including parsing, serialization, and validation. It is built on top of the powerful `LibXml2 <https://github.com/GNOME/libxml2>`_ library, which serves as the base for all XML-related operations.

**Key Features**:

  - In-memory representation of XML trees.
  - Parsing XML from files or strings.
  - Serializing XML trees to files or strings.
  - Validation of XML structure and content.
  - Utilizes the /LibXml2/ library for robust XML processing.

**Main Components**:

  1. ``TreeData``: Represents an XML tree in memory. Supports attributes, children, and content.
  2. ``TreeDataParser``: Parses XML into ``TreeData`` objects.
     
     - ``TreeDataFromFile``: Parse XML from a file.
     - ``TreeDataFromString``: Parse XML from a string.
  3. ``TreeDataSerialize``: Serializes ``TreeData`` objects to XML.
     
     - ``TreeDataToFile``: Serialize to a file.
     - ``TreeDataToString``: Serialize to a string.
  4. ``TreeDataValidate``: Validates XML structure and content. Example validations: no attributes, no children, specific child tags.

**Example**:

.. code-block:: c++

   #include <sup/xml/tree_data.h>
   #include <sup/xml/tree_data_parser.h>
   #include <sup/xml/tree_data_validate.h>
   #include <sup/xml/exceptions.h>
   #include <iostream>
   
   int main() {
       try {
           // XML string to parse
           std::string xml_string = R"(
               <Root>
                   <Child1 attribute="value1">Content1</Child1>
                   <Child2 attribute="value2">Content2</Child2>
               </Root>
           )";
   
           // Parse XML from string
           auto tree = sup::xml::TreeDataFromString(xml_string);
   
           // Print the root node name
           std::cout << "Root Node: " << tree->GetNodeName() << std::endl;
   
           // Iterate through children and print their details
           for (const auto& child : tree->Children()) {
               std::cout << "Child Node: " << child.GetNodeName() << std::endl;
               std::cout << "Content: " << child.GetContent() << std::endl;
               for (const auto& attribute : child.Attributes()) {
                   std::cout << "Attribute: " << attribute.first << " = " << attribute.second << std::endl;
               }
           }
   
           // Validate that the root node has no attributes
           sup::xml::ValidateNoAttributes(*tree);
   
           // Validate that the root node contains only allowed child tags
           std::vector<std::string> allowed_tags = {"Child1", "Child2"};
           sup::xml::ValidateAllowedChildTags(*tree, allowed_tags);
   
       } catch (const sup::xml::ValidationException& e) {
           std::cerr << "Validation Error: " << e.what() << std::endl;
       } catch (const sup::xml::ParseException& e) {
           std::cerr << "Parse Error: " << e.what() << std::endl;
       } catch (const std::exception& e) {
           std::cerr << "Error: " << e.what() << std::endl;
       }
   
       return 0;
   }
