#include <wifi_funcs.hpp>

const char *ssid = "iPhone de Julián ";
const char *password = "sanchez06";

Preferences prefs;
WebServer server(80);
DNSServer dns;

#if !CLOUD_SERVER

const char *mqtt_server = "raspberry.local";
const int mqtt_port = 8883;
const char *topic = "bike/mtls";

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char *ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIEATCCAumgAwIBAgIUYhVuRgeR247Wbu/gyz+zO+tYj8EwDQYJKoZIhvcNAQEL\n"
    "BQAwgY8xCzAJBgNVBAYTAkNPMRIwEAYDVQQIDAlBbnRpb3F1aWExETAPBgNVBAcM\n"
    "CE1lZGVsbGluMQ0wCwYDVQQKDARVZGVBMQwwCgYDVQQLDANJb1QxEDAOBgNVBAMM\n"
    "B0NBLUJpa2UxKjAoBgkqhkiG9w0BCQEWG2p1bGlhbi5zYW5jaGV6NkB1ZGVhLmVk\n"
    "dS5jbzAeFw0yNTEwMzEyMjA2MTNaFw0yNjEwMzEyMjA2MTNaMIGPMQswCQYDVQQG\n"
    "EwJDTzESMBAGA1UECAwJQW50aW9xdWlhMREwDwYDVQQHDAhNZWRlbGxpbjENMAsG\n"
    "A1UECgwEVWRlQTEMMAoGA1UECwwDSW9UMRAwDgYDVQQDDAdDQS1CaWtlMSowKAYJ\n"
    "KoZIhvcNAQkBFhtqdWxpYW4uc2FuY2hlejZAdWRlYS5lZHUuY28wggEiMA0GCSqG\n"
    "SIb3DQEBAQUAA4IBDwAwggEKAoIBAQCWuR8Rqof2OgVuTwBI8itA5tSHxgcdVF3J\n"
    "/G4G/hdGOkKqnRwHTU+zCgxqlvwrIw7y2nM0yoKFSHoiRDPBfdvevSA1kpDwmft6\n"
    "53Q1nFzAi6hVOPuo0iPuXrK2BVDYO9WicpEwhCc6I28O+iwLDlyvfbrvhw5WRiq9\n"
    "X+7KKBfOx+a0LWqnhgklIepNpGJvnrjCaniVB7LSHlZAgfeH/6u8LYBLT+7XnJoR\n"
    "H6Eggjbr8nn7q0iXs8Opdq2UgDIqpN0g/fjDJwcFr6BY2zZUqKoqQNBT3zsQyyEJ\n"
    "JX5HMA2JQ5yv5QirQjLFyvd1826dPG6AYZfhqoIoj+UMTqN2MwVXAgMBAAGjUzBR\n"
    "MB0GA1UdDgQWBBSMxCBRXfleNZiE2ztlgK2BfBE5xTAfBgNVHSMEGDAWgBSMxCBR\n"
    "XfleNZiE2ztlgK2BfBE5xTAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUA\n"
    "A4IBAQAeT4tQX3OjDfH3WC7s0KEoz8n7U3apj1aKLJvLVRSl3VGLCcI/cHnwQV4z\n"
    "91NnN2H5q8PhQymA5ReZHbW0pCYI1HrcU7fHqvJ4G+pXbj+tdM34ndoMxtMciiqx\n"
    "U3GOWGi8xCAr9A1cHHlrx24a3uHtxsgu7Lvo9AtOQzmVl1TOQMPdCiUKsuOQtBLk\n"
    "mjLQMN9es9WLd9j6u5xzYdrnfo7DgEPzQbwCtlfeUaLhomW59PnC4w0Dgd8WVE/A\n"
    "aPFqx8bWdpqoE3pmGVxtjWCf+BcCqQ4BivmCPLC9SWrQ0+8YSUWEQ7lgeAO4xXY6\n"
    "06Nd7WDq1xm5KteSGi9BIE+ZizTg\n"
    "-----END CERTIFICATE-----\n";

const char *client_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDrDCCApQCFH5LA+TPsNHvyHhjZ3k75hF0FAq6MA0GCSqGSIb3DQEBCwUAMIGP\n"
    "MQswCQYDVQQGEwJDTzESMBAGA1UECAwJQW50aW9xdWlhMREwDwYDVQQHDAhNZWRl\n"
    "bGxpbjENMAsGA1UECgwEVWRlQTEMMAoGA1UECwwDSW9UMRAwDgYDVQQDDAdDQS1C\n"
    "aWtlMSowKAYJKoZIhvcNAQkBFhtqdWxpYW4uc2FuY2hlejZAdWRlYS5lZHUuY28w\n"
    "HhcNMjUxMTAxMDQwMDM0WhcNMjYxMTAxMDQwMDM0WjCBlDELMAkGA1UEBhMCQ08x\n"
    "EjAQBgNVBAgMCUFudGlvcXVpYTERMA8GA1UEBwwITWVkZWxsaW4xDTALBgNVBAoM\n"
    "BFVkZUExDDAKBgNVBAsMA0lvVDEVMBMGA1UEAwwMRVNQMzItQ2xpZW50MSowKAYJ\n"
    "KoZIhvcNAQkBFhtqdWxpYW4uc2FuY2hlejZAdWRlYS5lZHUuY28wggEiMA0GCSqG\n"
    "SIb3DQEBAQUAA4IBDwAwggEKAoIBAQDiIu+l1ah8VvMdHNclpn2AQDG3a7UhNIkh\n"
    "evOMVnh7ivnOULl4+VEAFzE5FnmOHPnvrxO5zUC7sLzESubeHJUyc/jXDk5Djhix\n"
    "UzDYu0x6hDzWo7UbFmlv66I1CZuigE1GFAGVcaF6fdj4U82HkGAb6kd0PSBTwQPI\n"
    "eRh3GEj9SljY2s/wNqWoaxuFHMYfkmCoP8weV4LtXlvACFslITu8K1aDfGmLBQQ3\n"
    "7y/T0jFTIK7QlSsF2Vgxclzptt9LbZi0siEq5LyvkE52AmQrzBkoqSbbXszRSMLB\n"
    "pieQIGyY/28yiArvDg4wJfT+B1TQM6+dI7RCJNLZmwZ8XSiHt4LzAgMBAAEwDQYJ\n"
    "KoZIhvcNAQELBQADggEBADbMWJR3hqLhrV2oHZ/wRbRn2LISTN0B41HATi1wskx+\n"
    "fnueAnR/Cr34+GA+kSBY6SBe7g3jeZfRK5NtngTcq+vCKl+4oc3GwaSQ3xDYUljS\n"
    "9h1WgyyjoYB+7POCuNvpslK+6u9IkFQZDTBoz2f4NaNusxwIgKfoe+7nh/GNw8SY\n"
    "8Osm1FOWxyNYnlFi+AtXM3oHD6KrSOIomzGOWaH7U9zEbmGbh//CjLJ+y7AKJ/al\n"
    "3s4ylrclvnxfCsDCc8yrDLS/ZnY7To1GEa43L2SYzWbWuXQwQAxlvSluO7h8imPZ\n"
    "NJRDP2/Fq2Aq4oKt3ffDzZjOKBqUHJEJ6UKTgiCLRMY=\n"
    "-----END CERTIFICATE-----\n";

const char *client_key =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDiIu+l1ah8VvMd\n"
    "HNclpn2AQDG3a7UhNIkhevOMVnh7ivnOULl4+VEAFzE5FnmOHPnvrxO5zUC7sLzE\n"
    "SubeHJUyc/jXDk5DjhixUzDYu0x6hDzWo7UbFmlv66I1CZuigE1GFAGVcaF6fdj4\n"
    "U82HkGAb6kd0PSBTwQPIeRh3GEj9SljY2s/wNqWoaxuFHMYfkmCoP8weV4LtXlvA\n"
    "CFslITu8K1aDfGmLBQQ37y/T0jFTIK7QlSsF2Vgxclzptt9LbZi0siEq5LyvkE52\n"
    "AmQrzBkoqSbbXszRSMLBpieQIGyY/28yiArvDg4wJfT+B1TQM6+dI7RCJNLZmwZ8\n"
    "XSiHt4LzAgMBAAECggEAFXtcVarrnWg37xkpLQfyxLpHnshQeOeM0VpLN/0s5w7B\n"
    "A7YY1tYKuYRg5i66Ff1I48Ad339dxpaxaY10ZT9edi0DKhBo8EzxQ1y6REepEIhu\n"
    "xeff6ym6LJ1VsU3vQKtWHxg4CeWbmxwvMaPp+RaGssNfzbofnIuc6H0fpr/VWAF7\n"
    "lwyiIG2V6VmYkNQLOEcQRPTgj+45PocpnsNk0szqQQ8CmAwdT+YjJyro93sIsw2N\n"
    "UEn30/C4scxzwcEh7U5QwmYzn7Ut3mPxGf5Og8KX4lGK9FC/7GJRrktcdMLHfe6z\n"
    "/LWDaZsnL2+zsBx6c3u8kx/srR6EAQwUpkm/wvkilQKBgQD7zwQQbWwDUFjBBPUA\n"
    "o3PpWcHtGRYEYX1/8HWqXSimC36eEKZQCjTS7PBvsgDodNfwtKbE7p3nTYh/ovDP\n"
    "yccT1gtNjuVcHby2QkwuASlJgGPvgF7mmVnUMx9CSy+bBi5MEgHP+z1Pe28vkblo\n"
    "tzbllLbVDzZc5anKQS0OYl+jfQKBgQDl5oc8TqSPRF3iztE5dV0bKyF7dfUwb+RS\n"
    "tCqBc9BCpUDIZHsk5Z3Y7AncFOfltEL6npLuWW3WCDIfPZ3hBeC9+OiG6HfTl2XZ\n"
    "nCTA+ezQEncoTZ547b9JpzWfytE8hY5VQ5ZU+fOwYEZaQYMnSo7XKgD5KQWdSRxF\n"
    "fy3+qASrLwKBgE2urbhLqXjUPq09VEAgLNAeap80kABF+yuPCR3Ho5SmEye8RzKG\n"
    "E4nGot4ycelEsqGFebVjT79dwfBuWhAgVZQfAlmo7cUfP2qSxWGj5hLQ0wO9/Ijz\n"
    "Yiz+oHQZMdSHcZzyKTD6dw2gLMrmD+boLr1gxY6xJLo/98N3m+jGHG8ZAoGBAIQQ\n"
    "VBk5l+BQxpn2ckvOdoiX0ykHzqxfakp0AoLQk47lPPygc/jzCiTFGrjZLYlPbrZn\n"
    "B0JyDt1HYft+OsY7ySl35aMxwTvEGZbS0m4kY3v3T4nQZLtU33Cg4/YB3QYmNmgW\n"
    "ZjI0T18/Psq0HXIhq0JAd8SpYvdCO0JU14gU3nGzAoGBAO+OsZsNDV4OfM6DSmGv\n"
    "jBrTl9Za9bThcVdRNV2HFFONoXqE1fVwqHrYxYTtiUD2X91YiZIv/AQ8cc/1HXj/\n"
    "y80JDmKi4lA9SiT6WuTRvu6h4F6EzWHtacAHVAI38b4TE6QYSDGcyE8t8k6eio4B\n"
    "91UBK/Kn+zLedVoxE7ddCfvb\n"
    "-----END PRIVATE KEY-----\n";

//==================================================================================
// =============================== certifados Amazon ===============================
//==================================================================================
#elif CLOUD_SERVER
const char *mqtt_server = "a1lcoy4gm8z6za-ats.iot.us-east-2.amazonaws.com";
const int mqtt_port = 8883;
const char *topic = "bike/mtls";

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char *ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
    "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
    "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
    "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
    "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
    "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
    "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
    "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
    "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
    "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
    "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
    "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
    "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
    "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
    "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
    "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
    "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
    "rqXRfboQnoZsG4q5WTP468SQvvG5\n"
    "-----END CERTIFICATE-----\n";

const char *client_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDWTCCAkGgAwIBAgIUZR+EzpJWOSCkslkCiQofIct99jcwDQYJKoZIhvcNAQEL\n"
    "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
    "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MTAxODEzMjU1\n"
    "OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
    "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM5IqVZdftwyZhipBV+C\n"
    "uedpH0kspc2Bdx0j3Wp+J853SwmgWfQ+oxXdCIMmJRvlXIdxfXarIDqRpOC377EZ\n"
    "HkfQDgA2tTmjQ+tFWa85LnmuUjcdFZTevCo67ZxdUoW0dzuYV+7pHnOuA2pin161\n"
    "ZaSrXWtKyrzacK60h5xZY5X0hFHmxAr76c9AygVSO63SfkLiibtGq4JggiGccpcZ\n"
    "yK72r+Flodv50/Zy9nKQiFm8At/wQxAiyB3faDH1eqn2BUvpo+Neo/GVjKjml3Oi\n"
    "JXDwp0+qpWx/AsBpi3dxRBrzfwPsQm+0ZXEImrROnRPUAAkGzmw5NsclLXomYHS/\n"
    "ZM0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUxYbBafEY6V7pfSdUuddJn5QE5lwwHQYD\n"
    "VR0OBBYEFPIubCCom/E3+KgS1CGNgHOrg2Z8MAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
    "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQARFzXg4vzNQY8EIb17x2XRZBWs\n"
    "5w1NAKh96SSNaFxzOR1soFBdsl7xvgxQCAJTl90eQL/v6c1agKRYZ6iSlwHMKgoJ\n"
    "nhweiOPlQNE6NSZxrIxAGqgIuTOuQ8izVcKYjAgDux0JmLDmQgDa15iffOy0HvIk\n"
    "6KDmZhhi+P6/VWtrBygDxrtxNEVl8yPsnvTqhiA6H7tmvSDYml9zw1MZAhpxDXmx\n"
    "GD64v+6gVgZLd21MzRC80MlZUhLISADE19XRFr4SUZ9JDYVHx6lq8bDs4qUrk9XJ\n"
    "sQ87ej0HnASMc1dhCTaJ0VYQ54O0ZbGQP3U3A2lbMYRPehnfA4/g5dzsFYa2\n"
    "-----END CERTIFICATE-----\n";

const char *client_key =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEowIBAAKCAQEAzkipVl1+3DJmGKkFX4K552kfSSylzYF3HSPdan4nzndLCaBZ\n"
    "9D6jFd0IgyYlG+Vch3F9dqsgOpGk4LfvsRkeR9AOADa1OaND60VZrzkuea5SNx0V\n"
    "lN68KjrtnF1ShbR3O5hX7ukec64DamKfXrVlpKtda0rKvNpwrrSHnFljlfSEUebE\n"
    "Cvvpz0DKBVI7rdJ+QuKJu0argmCCIZxylxnIrvav4WWh2/nT9nL2cpCIWbwC3/BD\n"
    "ECLIHd9oMfV6qfYFS+mj416j8ZWMqOaXc6IlcPCnT6qlbH8CwGmLd3FEGvN/A+xC\n"
    "b7RlcQiatE6dE9QACQbObDk2xyUteiZgdL9kzQIDAQABAoIBABl9PP9g+iX3TEpL\n"
    "yFAu9Q21HZqLG8my76b4IYnhIMYQU4NS/wqBGuG+uCiMdBVoV/Ccg2ZAavERwg/K\n"
    "aqG1etpgyPoZyHbgx6mwng5awHVkpuAMhfgpNZL6SRYDKgk/bWQ9xM/Ommas39sr\n"
    "aInpXC/mxNpXGYoQa9AKmp8lJHsIzMGNbT9TPhHVANC7X3MGzj/ey2I3G9MUIhfo\n"
    "KFMIAa+PUa5AXu91aqjWSfdWO5hkiIUtid/uc2CANl2bEoMZ5QCZPheyT/hE5uLa\n"
    "pecCP/82JpBZ9NzC97H2O79Og/vfLrvZyBT0huYhdwJ0AKeY1/xg2W+F/4I12I41\n"
    "IbM56cECgYEA+I7S02Z1sAAxnPXn994G7F1mtOKMqiOFhwe4LESy0/9PfZ9omm6o\n"
    "4nGx+Lo/qrD1xWSmnyCZbNkr2C0eHPoGtdgxqfdmANCOrU+gOjba8ojwM5piuq1O\n"
    "53Wm63EGb/3pQxG0l7RtCnnZ3hM36mQC6zw0gsE+cTmKNhdZyjhGipUCgYEA1HXP\n"
    "gTcxJLiEYCef39bufkZwSOWLuRkOSGy1izM4tx5EFTAH3cRNpueLmnfBoXLm9kx/\n"
    "umXCB0ZbtwjnzQWXrTMoXowLnYv6EHuOy/TCbNZ7z+ABQXgw6gKPGwNfyVpU8pJq\n"
    "H3xZodIOxZwgBTVtk9g9Jwxtjnom/JGYjiHNm1kCgYANmLp9p/AuJYSRQn5i97lH\n"
    "OsDAilgJgpHuVgTEHyntIN03UP4G3KDOakvDAGU1MCLxnlTjBYR/CM42MBmimia9\n"
    "0cq9snPY/Y+geWeZZ6DIfacuG3blIU/cp3nvDSq9+5TD/3byiYMhfJLnbAO2BayH\n"
    "M67nrCx/QxeFZ/eYcUWilQKBgGYpyIBgI9d+itkZ9LzUQ5ahFHlZOBjnvYCCFcI7\n"
    "/TfA7AgsMnwnWxe5G95JW+jgscMaAgmFyQlqgcvAEob5+3ejs/PS7/rwRm0KnR3i\n"
    "MmPXt228T55Fx2+fj2Tz+GmAcHy8fx9o0kkIE2Zert8SVADV6IIQbdYnUvv8nxkp\n"
    "lA7xAoGBALgD1tP3zzqrHgNSA+BMxFWP0/NsO9mHSy6UlQFb5Ca9FCAk5X0SXKLI\n"
    "frwMSZHO7lDySWNJ7c1W7ZcRDBT79I0pvVbWP/CXuLngQOim2MZm2sLhBoAODdpl\n"
    "VwMMc0RsI6nzYkzhVPeVguj0IB+ZeK73wh8HrkUDHw1YHKMYie7U\n"
    "-----END RSA PRIVATE KEY-----\n";
#endif

void ask_credentials(String &ssid, String &pass)
{
    Serial.println("\nConfiguración WiFi:");
    Serial.print("Ingrese SSID: ");
    while (ssid.length() == 0)
    {
        if (Serial.available())
        {
            ssid = Serial.readStringUntil('\n');
            ssid.trim();
        }
    }
    Serial.print("\nIngrese Contraseña: ");
    while (pass.length() == 0)
    {
        if (Serial.available())
        {
            pass = Serial.readStringUntil('\n');
            pass.trim();
        }
    }
}

void setup_wifi()
{
    Serial.println();
    Serial.print("Cone3ctando a Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.print("\nWiFi conectado IP: ");
    Serial.println(WiFi.localIP());

    init_communications();
}

void syncTime()
{
    configTime(0, 0, "time.google.com", "co.pool.ntp.org");
    Serial.print("Esperando sincronización NTP");
    time_t now = time(nullptr);

    while (now < 8 * 3600 * 2)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    Serial.println("\nHora Sincronizada");
}

bool reconnect()
{
    while (!client.connected())
    {
        Serial.println("Conectando al broker MQTT...");

        if (client.connect("ESP32-Client"))
        {
            Serial.println("Conectado");
            return true;
        }
        else
        {

            Serial.print("Fallo, rc=");
            Serial.print(client.state());
            Serial.println(" Reintentando...");
            return false;
        }
    }
}

String msgToJson(const Telemetry_t &data)
{
    JsonDocument doc;

    doc["device_id"] = data.deviceId;
    doc["user_id"] = data.userId;
    doc["timestamp"] = data.timeStamp;
    doc["sequence_id"] = data.sequenceId;

    // AQUI VA UNA ESTRUCTURA SOLO PARA EL GPS!!!

    doc["gps"]["latitude"] = data.gpsData.latitude;
    doc["gps"]["longitude"] = data.gpsData.longitude;
    doc["gps"]["altitude"] = data.gpsData.altitude;
    doc["gps"]["velocity"] = data.gpsData.velocity;
    doc["gps"]["Hora local"] = data.gpsData.localTime;

    // AQUI VA UNA ESTRUCTURA SOLO PARA LA IMU

    doc["imu"]["accel_x"] = data.imuData.aX;
    doc["imu"]["accel_y"] = data.imuData.aY;
    doc["imu"]["accel_z"] = data.imuData.aZ;
    doc["imu"]["gyro_x"] = data.imuData.gX;
    doc["imu"]["gyro_y"] = data.imuData.gY;
    doc["imu"]["gyro_z"] = data.imuData.gZ;
    doc["imu"]["vibration"] = data.imuData.vibraciones;

    doc["angles"]["roll_deg"] = data.anglesData.roll;
    doc["angles"]["pitch_deg"] = data.anglesData.picht;

    // ALERTAS

    size_t estimated = measureJson(doc);
    Serial.printf("Tamaño estimado JSON: %u bytes\n", estimated);

    String out;
    serializeJson(doc, out);

    Serial.print("Tamaño JSON: ");
    Serial.print(out.length());
    Serial.println(" bytes");
    return out;
}

bool sendData(const String &data)
{
    bool ret = false;
    if (!client.connected())
    {
        ret = reconnect();
        if (ret == false)
        {
            return false;
        }
    }

    client.loop();

    bool ok = client.publish(topic, data.c_str());

    if (ok){
        Serial.println("Publicación exitosa");
        return true;
    }
    else {
        Serial.println("Error en publish()");
        return false;
    }

}

void init_communications()
{
    syncTime();

    espClient.setCACert(ca_cert);
    espClient.setCertificate(client_cert);
    espClient.setPrivateKey(client_key);
    client.setServer(mqtt_server, mqtt_port);

    reconnect();
}

String htmlPage(const String &currentSSID, const String &currentName)
{
    return R"(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="utf-8">
    <title>Configuración WiFi</title>
    <style>
      body { font-family: Arial; text-align: center; margin-top: 40px; }
      input { padding: 10px; margin: 8px; width: 240px; }
      button { padding: 12px 30px; margin-top: 15px; font-size: 16px; }
      .box { width: 350px; margin: auto; padding: 20px; border-radius: 12px;
             border: 1px solid #ddd; background: #fafafa; }
    </style>
  </head>
  <body>
    <div class="box">
      <h2>Configuración del Dispositivo</h2>

      <p><b>SSID actual:</b> )" +
           currentSSID + R"(</p>
      <p><b>Nombre actual:</b> )" +
           currentName + R"(</p>

      <form action="/save" method="post">
        <input name="device" placeholder="Nombre del dispositivo"><br>
        <input name="ssid" placeholder="SSID WiFi"><br>
        <input name="pass" type="password" placeholder="Contraseña"><br>
        <button type="submit">Guardar</button>
      </form>
    </div>
  </body>
  </html>
  )";
}

void setupCaptiveRedirects()
{
    auto redirectToRoot = []()
    {
        server.sendHeader("Location", "/", true);
        server.send(302, "text/plain", "");
    };

    server.on("/generate_204", redirectToRoot);        // Android
    server.on("/gen_204", redirectToRoot);             // Otros Android
    server.on("/hotspot-detect.html", redirectToRoot); // iOS / macOS
    server.on("/library/test/success.html", redirectToRoot);
    server.on("/fwlink", redirectToRoot); // Windows
}

void handleSave()
{
    String name = server.arg("device");
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");

    if (ssid.isEmpty() || pass.isEmpty())
    {
        server.send(400, "text/html", "<h3>Error: SSID o contraseña vacíos.</h3>");
        return;
    }

    if (name.isEmpty())
        name = "ESP32";

    prefs.begin("wifi", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.putString("name", name);
    prefs.end();

    server.send(200, "text/html",
                "<h3>Datos guardados correctamente.<br>Reiniciando...</h3>");

    delay(2000);
    ESP.restart();
}

void startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP32_Setup", "12345678");

    IPAddress apIP = WiFi.softAPIP();
    dns.start(DNS_PORT, "*", apIP);

    Serial.println("\n=== MODO CONFIGURACIÓN ===");
    Serial.println("AP: ESP32_Setup");
    Serial.print("IP: ");
    Serial.println(apIP);

    // Rutas del portal cautivo
    setupCaptiveRedirects();

    // Ruta principal
    server.on("/", []()
              {
    prefs.begin("wifi", true);
    String ssid = prefs.getString("ssid", "(no configurado)");
    String name = prefs.getString("name", "(sin nombre)");
    prefs.end();

    server.send(200, "text/html", htmlPage(ssid, name)); });

    // Ruta de guardado
    server.on("/save", handleSave);

    server.begin();
    Serial.println("Servidor HTTP y DNS iniciados.");
}

bool connectToWiFi()
{
    prefs.begin("wifi", true);
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    String name = prefs.getString("name", "ESP32");
    prefs.end();

    if (ssid.isEmpty())
        return false;

    Serial.printf("Intentando conectar a '%s'...\n", ssid.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    for (int i = 0; i < 20; i++)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Conexión exitosa!");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            Serial.printf("Nombre: %s\n", name.c_str());
            return true;
        }
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nNo se pudo conectar.");
    return false;
}