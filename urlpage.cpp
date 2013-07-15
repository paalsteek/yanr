#include "urlpage.h"
#include "ui_urlpage.h"

#include <QtCore/QDebug>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QAuthenticator>

/*
 * According to RFC 3986 a URI has to match the following:
 *
 * URI         = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
 *
 * hier-part   = "//" authority path-abempty
 *             / path-absolute
 *             / path-rootless
 *             / path-empty
 *
 * scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 *
 * authority   = [ userinfo "@" ] host [ ":" port ]
 *
 * userinfo    = *( unreserved / pct-encoded / sub-delims / ":" )
 *
 * host        = IP-literal / IPv4address / reg-name
 *
 * IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
 * IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
 *
 * IPv6address =                            6( h16 ":" ) ls32
 *             /                       "::" 5( h16 ":" ) ls32
 *             / [               h16 ] "::" 4( h16 ":" ) ls32
 *             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
 *             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
 *             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
 *             / [ *4( h16 ":" ) h16 ] "::"              ls32
 *             / [ *5( h16 ":" ) h16 ] "::"              h16
 *             / [ *6( h16 ":" ) h16 ] "::"
 *
 * ls32        = ( h16 ":" h16 ) / IPv4address
 *             ; least-significant 32 bits of address
 *
 * h16         = 1*4HEXDIG
 *             ; 16 bits of address represented in hexadecimal
 *
 * IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
 *
 * dec-octet   = DIGIT                 ; 0-9
 *             / %x31-39 DIGIT         ; 10-99
 *             / "1" 2DIGIT            ; 100-199
 *             / "2" %x30-34 DIGIT     ; 200-249
 *             / "25" %x30-35          ; 250-255
 *
 * reg-name    = *( unreserved / pct-encoded / sub-delims )
 *
 * port        = *DIGIT
 *
 * path          = path-abempty    ; begins with "/" or is empty
 *               / path-absolute   ; begins with "/" but not "//"
 *               / path-noscheme   ; begins with a non-colon segment
 *               / path-rootless   ; begins with a segment
 *               / path-empty      ; zero characters
 *
 * path-abempty  = *( "/" segment )
 * path-absolute = "/" [ segment-nz *( "/" segment ) ]
 * path-noscheme = segment-nz-nc *( "/" segment )
 * path-rootless = segment-nz *( "/" segment )
 * path-empty    = 0<pchar>
 *
 * segment       = *pchar
 * segment-nz    = 1*pchar
 * segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
 *               ; non-zero-length segment without any colon ":"
 *
 * query       = *( pchar / "/" / "?" )
 *
 * fragment    = *( pchar / "/" / "?" )
 *
 * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
 *
 * pct-encoded = "%" HEXDIG HEXDIG
 *
 * reserved    = gen-delims / sub-delims
 *
 * gen-delims  = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 *
 * sub-delims  = "!" / "$" / "&" / "'" / "(" / ")"
 *             / "*" / "+" / "," / ";" / "="
 *
 * unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
 */

QString UrlPage::alphaSym = "a-zA-Z";
QString UrlPage::digitSym = "\\d";
QString UrlPage::hexdigExp = "[0-9a-fA-F]";
QString UrlPage::unreservedExp = "[" + alphaSym + digitSym + "\\._~-]";
QString UrlPage::subdelimsExp = "[!\\$&'\\(\\)\\*\\+,;=]";
QString UrlPage::gendelimsExp = "[:/\\?#\\[\\]@]";
QString UrlPage::reservedExp = gendelimsExp + "|" + subdelimsExp;
QString UrlPage::pctencodedExp = "%" + hexdigExp + hexdigExp;
QString UrlPage::pcharExp = unreservedExp + "|" + pctencodedExp + "|" + subdelimsExp + "|" + "[:@]";
QString UrlPage::fragmentExp = "(" + pcharExp + "|" + "[/\\?]" + ")*";
QString UrlPage::queryExp = "(" + pcharExp + "|" + "[/\\?]" + ")*";
QString UrlPage::segmentExp = "(" + pcharExp + ")*";
QString UrlPage::segmentnzExp = "(" + pcharExp + ")+";
QString UrlPage::segmentnzncExp = "(" + unreservedExp + "|" + pctencodedExp + "|" + subdelimsExp + "|" + "[@]" + ")+";
QString UrlPage::pathemptyExp = "";
QString UrlPage::pathrootlessExp = segmentnzExp + "(/|" + segmentExp + ")*";
QString UrlPage::pathnoschemeExp = segmentnzncExp + "(/|" + segmentExp + ")*";
QString UrlPage::pathabsoluteExp = "/(" + segmentnzExp + "(/" + segmentExp + ")*)?";
QString UrlPage::pathabemptyExp = "(/" + segmentExp + ")*";
QString UrlPage::pathExp = "(" + pathabemptyExp + "|" + pathabsoluteExp + "|" + pathnoschemeExp + "|" + pathrootlessExp + "|" + pathemptyExp + ")";
QString UrlPage::portExp = digitSym + "*";
QString UrlPage::regnameExp = "(" + unreservedExp + "|" + pctencodedExp + "|" + subdelimsExp + ")*";
QString UrlPage::decoctetExp = "(" + digitSym + "|" + "[1-9]" + digitSym + "|" + "1" + digitSym + "{2}" + "|" + "2[0-4]" + digitSym + "|" + "25[0-5]" + ")";
QString UrlPage::ipv4addrExp = decoctetExp + "\\." + decoctetExp + "\\." + decoctetExp + "\\." + decoctetExp;
QString UrlPage::h16Exp = hexdigExp + "{1,4}";
QString UrlPage::ls32Exp = "((" + h16Exp + ":" + h16Exp + ")|" + ipv4addrExp + ")";
QString UrlPage::ipv6addrExp = "((" + h16Exp + ":){6}" + ls32Exp + "|" + \
        "::" + "(" + h16Exp + ":){5}" + ls32Exp + "|" + \
        "(" + h16Exp + ")?" + "::" + "(" + h16Exp + ":){4}" + ls32Exp + "|" + \
        "(" + "(" + h16Exp + ":){,1}" + h16Exp + ")?" + "::" + "(" + h16Exp + ":){3}" + ls32Exp + "|" + \
        "(" + "(" + h16Exp + ":){,2}" + h16Exp + ")?" + "::" + "(" + h16Exp + ":){2}" + ls32Exp + "|" + \
        "(" + "(" + h16Exp + ":){,3}" + h16Exp + ")?" + "::" + h16Exp + ":" + ls32Exp + "|" + \
        "(" + "(" + h16Exp + ":){,4}" + h16Exp + ")?" + "::" + ls32Exp + "|" + \
        "(" + "(" + h16Exp + ":){,5}" + h16Exp + ")?" + "::" + h16Exp + "|" + \
        "(" + "(" + h16Exp + ":){,6}" + h16Exp + ")?" + "|" + \
        ")" + "(%" + "([" + alphaSym + digitSym + "])+)?" ;
QString UrlPage::ipvfutureExp = "v" + hexdigExp + "?\\.(" + unreservedExp + "|" + subdelimsExp + "|:)?";
QString UrlPage::ipliteralExp = "\\[(" + ipv6addrExp + "|" + ipvfutureExp + ")\\]";
QString UrlPage::hostExp = "(" + ipliteralExp + "|" + ipv4addrExp + "|" + regnameExp + ")";
QString UrlPage::userinfoExp = "(" + unreservedExp + "|" + pctencodedExp + "|" + subdelimsExp + "|:)*";
QString UrlPage::authorityExp = "(" + userinfoExp + "@)?" + hostExp + "(:" + portExp + ")?";
QString UrlPage::schemeExp = "[" + alphaSym + "][" + alphaSym + digitSym + "\\+\\.-]*";
QString UrlPage::hierpartExp = "(//" + authorityExp + pathabemptyExp + "|" + pathabsoluteExp + "|" + pathrootlessExp + "|" + pathemptyExp + ")";
QString UrlPage::uriExp = schemeExp + ":" + hierpartExp + "(\\?" + queryExp + ")?(#" + fragmentExp + ")?";

UrlPage::UrlPage(QWidget *parent)
    : QWizardPage(parent)
    , _ui(new Ui::UrlPage)
    , _complete(false)
    , _nam(new QNetworkAccessManager)
{
    _ui->setupUi(this);

    connect(_ui->urlEdit, SIGNAL(textEdited(QString)), this, SLOT(checkUrl(QString)));

    registerField("url", _ui->urlEdit);

    connect(_nam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authRequired(QNetworkReply*,QAuthenticator*)));
    connect(_nam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslError(QNetworkReply*,QList<QSslError>)));
    connect(_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(checkFinished(QNetworkReply*)));

    testURIExp();
}

UrlPage::~UrlPage()
{
    delete _ui;
    emit cancelRequests();
}

bool UrlPage::isComplete() const
{
    qDebug() << "isComplete";
    return _complete;
}

void UrlPage::checkUrl(QString url)
{
    emit cancelRequests();
    _complete = false;
    this->completeChanged();
    qDebug() << "Checking" << url;

    QRegExp srx("^(http|https|ftp)://.*$");
    if ( !srx.exactMatch(url))
        url = "http://" + url;
    QRegExp rx("^" + uriExp + "$");
    if ( rx.exactMatch(url) )
    {
        // TODO: delay network check to reduce network traffic
        qDebug() << "regex matched";
        QNetworkReply* reply = _nam->head(QNetworkRequest(QUrl(url)));
        connect(this, SIGNAL(cancelRequests()), reply, SLOT(abort()));
    }
}

void UrlPage::authRequired(QNetworkReply *reply, QAuthenticator *auth)
{
    qDebug() << "authRequired for" << reply->url() << "with options" << auth->options();
}

void UrlPage::sslError(QNetworkReply *reply, QList<QSslError> errors)
{
    qDebug() << "sslError while checking" << reply->url();
    for ( QList<QSslError>::Iterator it = errors.begin(); it != errors.end(); it++)
    {
        qDebug() << it->errorString();
    }
}

void UrlPage::checkFinished(QNetworkReply *reply)
{
    qDebug() << "checkFinished";
    if ( reply->error() )
        qDebug() << "Check failed:" << "GET" << reply->url().toString() << ":" << reply->errorString();
    else
    {
        qDebug() << "Check successful:" << "GET" << reply->url().toString() << ":" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        _complete = true;
        this->completeChanged();
    }
}

void UrlPage::testURIExp()
{
    qDebug() << "running testURIExp";
    QString scheme = "^" + schemeExp + "$";
    QRegExp schemeRx(scheme);
    if ( !schemeRx.exactMatch("http") )
        qDebug() << "schemeExp failed";
    QString authority = "^" + authorityExp + "$";
    QRegExp authorityRx(authority);
    if ( !authorityRx.exactMatch("www.chaos-inkl.de") )
        qDebug() << "authorityExp failed";
    QString pathabempty = "^" + pathabemptyExp + "$";
    QRegExp pathabemptyRx(pathabempty);
    if ( !pathabemptyRx.exactMatch("/wiki/") )
        qDebug() << "pathabemptyExp failed";
    QString regname = "^" + regnameExp + "$";
    QRegExp regnameRx(regname);
    if ( !regnameRx.exactMatch("www.chaos-inkl.de") )
        qDebug() << "hierpartExp failed";
    QString hierpart = "^" + hierpartExp + "$";
    QRegExp hierpartRx(hierpart);
    if ( !hierpartRx.exactMatch("//www.chaos-inkl.de/wiki/") )
        qDebug() << "hierpartExp failed";
    QString query = "^" + queryExp + "$";
    QRegExp queryRx(query);
    if ( !queryRx.exactMatch("title=Spezial:Feed%2C") )
        qDebug() << "queryExp failed";
    QString fragment = "^" + fragmentExp + "$";
    QRegExp fragmentRx(fragment);
    if ( !fragmentRx.exactMatch("title=Spezial:Feed%2C") )
        qDebug() << "fragementExp failed";
    QString uriBase = "^" + schemeExp + ":" + hierpartExp + "$";
    QRegExp uriBaseRx(uriBase);
    if ( !uriBaseRx.exactMatch("http://chaos-inkl.de/") )
        qDebug() << "uriBaseRx failed";
}
