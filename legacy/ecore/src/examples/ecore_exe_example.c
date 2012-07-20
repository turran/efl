/**
   Compile with gcc -o ecore_exe_example ecore_exe_example.c `pkg-config --cflags --libs ecore`
 */

#include <stdio.h>
#include <string.h>
#include <Ecore.h>

#define BUFFER_SIZE 1024

static Eina_Bool
_msg_from_child_handler(void *data, int type, void *event)
{
   Ecore_Exe_Event_Data *dataFromProcess = (Ecore_Exe_Event_Data *)event;
   char msg[BUFFER_SIZE];

   if (dataFromProcess->size >= (BUFFER_SIZE - 1))
     {
        fprintf(stdout, "Data too big for bugger. error\n");
        return ECORE_CALLBACK_DONE;
     }
   
   strncpy(msg, dataFromProcess->data, dataFromProcess->size);
   msg[dataFromProcess->size = 0;
       
   if (strcmp(msg, "quit") == 0)
     {
        fprintf(stdout, "My child said to me, QUIT!\n");
        ecore_main_loop_quit();
     }
   else
     fprintf(stdout, "I received a message from my child: %s\n", msg);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_sendMessage(void *data)
{
   static int numberOfMessages = 0;
   Ecore_Exe *childHandle = (Ecore_Exe *)data;
   char msg[BUFFER_SIZE];

   sprintf(msg, " Message: %d\n", numberOfMessages);
   numberOfMessages++;

   if (ecore_exe_send(childHandle, msg, strlen(msg)) != EINA_TRUE)
     fprintf(stderr, "Could not send my name to the child\n");
   else
     fprintf(stdout,
             "I'm the father and I sent this message to the child: %s\n", msg);

   return ECORE_CALLBACK_RENEW;
}

int
main(int argc, char **argv)
{
   pid_t childPid;
   Ecore_Exe *childHandle;

   if (!ecore_init())
     goto exit;

   childHandle = ecore_exe_pipe_run("./ecore_exe_example_child",
                                    ECORE_EXE_PIPE_WRITE |
                                    ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                                    ECORE_EXE_PIPE_READ, NULL);

   if (childHandle == NULL)
     {
        fprintf(stderr, "Could not create a child process!\n");
        goto ecore_shutdown;
     }

   childPid = ecore_exe_pid_get(childHandle);

   if (childPid == -1)
     fprintf(stderr, "Could not retrive the PID!\n");
   else
     fprintf(stdout, "The child process has PID:%d\n", childPid);

   ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _msg_from_child_handler, NULL);
   ecore_timer_add(1, _sendMessage, childHandle);

   ecore_main_loop_begin();

   ecore_exe_free(childHandle); //This will not affect the child process

   ecore_shutdown();

   return EXIT_SUCCESS;

ecore_shutdown:
   ecore_shutdown();

exit:
   return EXIT_FAILURE;
}

