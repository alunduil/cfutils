/**
 * @copyright 2013
 * @author Alex Brandt
 *
 * cf-utils is freely distributable under the terms of an MIT-style license.
 * See COPYING or http://www.opensource.org/licenses/mit-license.php.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../uhttpc/request.h"
#include "../uhttpc/response.h"
#include "../uhttpc/actions.h"

#include "container.h"

/**
 * Upstream container property notes:
 *
 * @note Maximum of 500,000 containers.
 * @note No / in container names.
 * @note Less than 256 bytes in length.
 * @note Container name length is after URL encoding.
 */

/**
 * @todo URL Encode Names
 * @todo UTF-8 Encode Names
 * @todo 256 Bytes in Name after URL encoding
 */

/**
 * Example Request:
 *
 * GET /v1.0/jdoe HTTP/1.1
 * Host: storage.clouddrive.com
 * X-Auth-Token: eaaafd18-0fed-04b3a-81b4-663c99ec1cbb
 * Content-Type: text/plain; charset=UTF-8
 * Content-Length: 32
 *
 * Example Response:
 *
 * images
 * movies
 * documents
 * backups
 */
const unsigned long int get_container_names ( const Account * account, Container containers[], unsigned long int * length ) {
    http_request * req = http_request_create();
    const http_response * resp;

    char name_length[10];

    char * haystack = NULL;
    char * name = NULL;

    add_header_to_request ( req, "X-Auth-Token", account->token );

    if ( *length > 0 ) {
        sprintf ( name_length, "%lu", *length );
        add_query_parameter_to_request ( req, "limit", name_length );
    }

    req->url = account->management_url;

    resp = http ( req );

    *length = 0;

    switch ( atoi ( resp->status_code ) ) {
    case 204:
        break;
    case 200:
        haystack = req->body;

        do {
            name = strtok ( haystack, "\n" );

            if ( name == NULL || _is_name_element_of_containers(name, containers, *length) ) break;

            haystack = NULL;

            containers = realloc ( containers, ++ ( *length ) * sizeof ( Container ) );
            strncpy ( containers[*length].name, name, 256 );
        } while ( 1 );

        break;
    }

    http_request_free ( ( void * ) req );
    http_response_free ( ( void * ) resp );

    return * length;
}

const unsigned char _is_name_element_of_containers ( const char name[], const Container containers[], const long unsigned int length ) {
	long unsigned int index = 0;

	unsigned char ret = 0;

	do {
		if ( containers[index].name == name ) ++ret;
	} while (++index < length);

	return ret;
}

const unsigned long int get_container_details ( const Account * account, Container containers[], unsigned long int * length ) {
    http_request * req = http_request_create();
    const http_response * resp;


}
