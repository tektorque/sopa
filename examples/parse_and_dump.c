#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sopa/sopa.h>

static SopaParser *parser;
static SopaDocument *document;

static const char *html = "                                         \
<!DOCTYPE html>                                                     \n \
<html>                                                              \n \
  <title>dump example</title>                                       \n \
  <link rel=\"stylesheet\" type=\"text/css\" href=\"theme.css\"/>   \n \
  <script type=\"text/javascript\">                                 \n \
    for (let i = 0; i < 10; i++)                                    \n \
      alert ('count with me ' + i);                                 \n \
  </script>                                                         \n \
  <body>                                                            \n \
    <!--This is a comment.-->                                       \n \
    <div id=\"first_div\" class=\"foo bar\">                        \n \
      <p>This is some text in a paragraph.</p>                      \n \
      <br/>                                                         \n \
      <p>This is some text in a paragraph.</p>                      \n \
    </div>                                                          \n \
  </body>                                                           \n \
</html>                                                             \n \
";

int
main (int argc, char **argv)
{
  gchar *dump;
  GError *error = NULL;

  parser = sopa_parser_new ();
  document = sopa_parser_parse (parser,
                                html,
                                strlen (html),
                                &error);

  if (document == NULL)
    {
      g_warning (error->message);
      g_error_free(error);
    }
  else
    {
      dump = sopa_element_to_string (SOPA_ELEMENT (document), 4);
      g_print("Dump:\n%s", dump);
      g_free (dump);
      g_object_unref (document);
    }

  g_object_unref (parser);

  return EXIT_SUCCESS;
}
