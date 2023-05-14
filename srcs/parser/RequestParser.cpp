//
// Created by rmore on 13/05/2023.
//

#include "../../includes/parser/RequestParser.h"

RequestParser::RequestParser() : state(RequestMethodStart), _content_size(0){}

RequestParser::~RequestParser() {}

RequestParser::ParseState RequestParser::parse(RequestData &req, char c) {
    return consume(req, c);
}


//Colocar o parse Result dentro do State, virar ParseState. Mudar os parâmetros de parse e cosume para (RequestData &req, char c)
//Fazer um loop dentro da função read_request lendo byte a byte até o estado se tornar ParsingCompleted ou FileUpload
//Salvar o ParseState em uma variável global da classe Event
//Na escola de qual fluxo seguir:
//  Se for ParsingCompleted, continua o GET request normalmente
//  Se for FileUpload, setar chama a função read_upload_file (Tentar dar uma forma conseguir ler dados binários)
RequestParser::ParseState RequestParser::consume(RequestData &req, char input) {

    switch (state) {
        case RequestMethodStart:
            if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
                state = RequestMethod;
                req.methodPushBack(input);
            }
            break;
        case RequestMethod:
            if (input == ' ') {
                state = RequestUriStart;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
                req.methodPushBack(input);
            }
            break;
        case RequestUriStart:
            if (isControl(input)) {
                return ParsingError;
            } else {
                state = RequestUri;
                req.uriPushBack(input);
            }
            break;
        case RequestUri:
            if (input == ' ') {
                state = RequestHttpVersion_h;
            } else if (input == '\r') {
                req.setVersionMajor(0);
                req.setVersionMinor(9);

                return ParsingCompleted;
            } else if (isControl(input)) {
                return ParsingError;
            } else {
                req.uriPushBack(input);
            }
            break;
        case RequestHttpVersion_h:
            if (input == 'H') {
                state = RequestHttpVersion_ht;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_ht:
            if (input == 'T') {
                state = RequestHttpVersion_htt;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_htt:
            if (input == 'T') {
                state = RequestHttpVersion_http;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_http:
            if (input == 'P') {
                state = RequestHttpVersion_slash;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_slash:
            if (input == '/') {
                req.setVersionMajor(0);
                req.setVersionMinor(0);
                state = RequestHttpVersion_majorStart;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_majorStart:
            if (isDigit(input)) {
                req.setVersionMajor(input - '0');
                state = RequestHttpVersion_major;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_major:
            if (input == '.') {
                state = RequestHttpVersion_minorStart;
            } else if (isDigit(input)) {
                req.setVersionMajor(req.getVersionMajor() * 10 + input - '0');
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_minorStart:
            if (isDigit(input)) {
                req.setVersionMinor(input - '0');
                state = RequestHttpVersion_minor;
            } else {
                return ParsingError;
            }
            break;
        case RequestHttpVersion_minor:
            if (input == '\r') {
                state = ResponseHttpVersion_newLine;
            } else if (isDigit(input)) {
                req.setVersionMinor(req.getVersionMinor() * 10 + input - '0');
            } else {
                return ParsingError;
            }
            break;
        case ResponseHttpVersion_newLine:
            if (input == '\n') {
                state = HeaderLineStart;
            } else {
                return ParsingError;
            }
            break;
        case HeaderLineStart:
            if (input == '\r') {
                state = ExpectingNewline_3;
            } else if (!req.getHeaders().empty() && (input == ' ' || input == '\t')) {
                state = HeaderLws;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
               req.headerItemPushBackNameReserve(input);
               state = HeaderName;
            }
            break;
        case HeaderLws:
            if (input == '\r') {
                state = ExpectingNewline_2;
            } else if (input == ' ' || input == '\t') {
            } else if (isControl(input)) {
                return ParsingError;
            } else {
                state = HeaderValue;
                req.headerItemPushBackValue(input);
            }
            break;
        case HeaderName:
            if (input == ':') {
                state = SpaceBeforeHeaderValue;
            } else if (!isChar(input) || isControl(input) || isSpecial(input)) {
                return ParsingError;
            } else {
                req.headerItemPushBackName(input);
            }
            break;
        case SpaceBeforeHeaderValue:
            if (input == ' ') {
                state = HeaderValue;
            } else {
                return ParsingError;
            }
            break;
        case HeaderValue:
            if (input == '\r') {
                if (req.getMethod() == "POST" || req.getMethod() == "PUT") {
                    RequestData::HeaderItem h = req.getHeaders().back();

                    if (strcasecmp(h.name.c_str(), "Content-Length") == 0) {
                        _content_size = atoi(h.value.c_str());
                        req.reserveContent(_content_size);
                    } else if (strcasecmp(h.name.c_str(), "Transfer-Encoding") == 0) {
                        if (strcasecmp(h.value.c_str(), "chunked") == 0)
                            return ParsingError;
                    }
                }
                state = ExpectingNewline_2;
            } else if (isControl(input)) {
                return ParsingError;
            } else {
                req.headerItemPushBackValue(input);
            }
            break;
        case ExpectingNewline_2:
            if (input == '\n') {
                state = HeaderLineStart;
            } else {
                return ParsingError;
            }
            break;
        case ExpectingNewline_3: {
            std::vector<RequestData::HeaderItem> headers = req.getHeaders();
            std::vector<RequestData::HeaderItem>::iterator it = std::find_if(headers.begin(),
                                                                             headers.end(),
                                                                             checkIfConnection);

            if (it != headers.end()) {
                if (strcasecmp(it->value.c_str(), "Keep-Alive") == 0) {
                    req.setKeepAlive(true);
                } else  // == Close
                {
                    req.setKeepAlive(false);
                }
            } else {
                if (req.getVersionMajor() > 1 || (req.getVersionMajor() == 1 && req.getVersionMinor() == 1))
                    req.setKeepAlive(true);
            }

            if (_content_size == 0) {
                if (input == '\n')
                    return ParsingCompleted;
                else
                    return ParsingError;
            } else {
                state = Post;
            }
            break;
        }
        case Post:
            //TODO: Fazer o parse do body, retornar novos atributos na classe RequestData
            //  Ao invés de retornar File Upload, retornar parsing completed
            //  E na classe RequestData adicionar 3 bool flags: File Request (GET) , File Upload (POST) e File Delete (DELETE).
            //  Tem que ter apenas uma verdadeira, caso contrário, retornar ParsingError
            return FileUpload;
        default:
            return ParsingIncompleted;
    }

    return state;
}

bool RequestParser::checkIfConnection(const RequestData::HeaderItem &item) {
    return strcasecmp(item.name.c_str(), "Connection") == 0;
}

bool RequestParser::isChar(int c) {
    return c >= 0 && c <= 127;
}

bool RequestParser::isControl(int c) {
    return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::isDigit(int c) {
    return c >= '0' && c <= '9';
}

bool RequestParser::isSpecial(int c) {
    switch (c) {
        case '(':
        case ')':
        case '<':
        case '>':
        case '@':
        case ',':
        case ';':
        case ':':
        case '\\':
        case '"':
        case '/':
        case '[':
        case ']':
        case '?':
        case '=':
        case '{':
        case '}':
        case ' ':
        case '\t':
            return true;
        default:
            return false;
    }
}
