// Source: https://specifications.freedesktop.org/mpris-spec/2.2/Player_Interface.html

#include <dbus/dbus.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void program(int type, char **parameter)
{
	printf("Program Start");
	DBusConnection *connection;
	DBusError error;

	dbus_error_init(&error);

	// Versuche eine Verbindung zum DBUS aufzubauen
	connection = dbus_bus_get(DBUS_BUS_SESSION,
							  &error);

	// .. und ggf auf Fehler zu reagieren
	if (dbus_error_is_set(&error) ||
		connection == NULL)
	{
		printf("Error: System Bus Connection Error");
		dbus_error_free(&error);
	}

	// Hole den Verbindungsnamen
	// https://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8a9024c78c4ea89b6271f19dbc7861b2
	int repext = dbus_bus_request_name(connection,
									   "org.mpris.MediaPlayer2.spotify",
									   DBUS_REQUEST_NAME_REPLY_EXISTS,
									   &error);

	// .. und ggf auf Fehler zu reagieren
	if (dbus_error_is_set(&error))
	{
		printf("Error: Bus Name Error");
		dbus_error_free(&error);
	}

	// Wenn nicht Inhaber der Verbindung
	/*if (repext != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
	{
		printf("Error: Primary Owner Error");
		exit(1);
	}*/

	DBusMessage *message;
	DBusMessageIter arguments;
	DBusPendingCall *pending;

	message = dbus_message_new_method_call("org.mpris.MediaPlayer2.spotify",
										   "/org/mpris/MediaPlayer2",
										   "org.mpris.MediaPlayer2.Player",
										   parameter[0]);

	if (message == NULL)
	{
		printf("Error: Message NULL");
		exit(1);
	}

	// method with parameters
	if (type > 0)
	{
		// OpenUri
		if (strcmp(
				parameter[0],
				"OpenUri"))
		{
			dbus_message_iter_init_append(message,
										  &arguments);

			if (dbus_message_iter_append_basic(&arguments,
											   DBUS_TYPE_STRING,
											   parameter[1]) == FALSE)
			{
				printf("Error: OOM");
			}
		}
	}

	if (dbus_connection_send_with_reply(connection,
										message,
										&pending,
										-1) == FALSE)
	{
		printf("Error: OOM");
	}

	if (pending == NULL)
	{
		printf("Error: Pending NULL");
	}

	// Flush und free

	dbus_connection_flush(connection);
	printf("Info: DBUS Message sent");

	dbus_message_unref(message);

	// Thread auf Pending warten lassen
	dbus_pending_call_block(pending);

	// Finished because Spotify Methods wont sent any further information
}

int main(int argc, char **argv)
{
	program(1, (char *[]){
				   "OpenUri",
				   "spotify:track:4jLv3tDBu8ww2R07DvL12s"});
	return 0;
}
