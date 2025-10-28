// Wifi com 2 redes
// const char *ssid[] = { "Zezinho", "jose" };
// const char *ssid[] = { "Nhazinha" };
const char *ssid[] = {"NHAZINHA (EVENTOS)_EXT", "NHAZINHA", "NHAZINHA (EVENTOS 1)"}; //-> Usar essa
// const char *ssid[] = { "Nhazinha Extendida" };
// const char *ssid[] = { "NHAZINHA (EVENTOS)" };
// const char *ssid[] = { "SALLESB" };

const char *password = "014280639";
// const char *password = "carlamarco64";
const int numNetworks = sizeof(ssid) / sizeof(ssid[0]);

// Laravel Project
const char *liveDatApi = "https://podoragreenhouse.com.br/api/v0/livedata";
const char *postAvgAPI = "https://podoragreenhouse.com.br/api/v0/avgdata";
const char *configAPI = "https://podoragreenhouse.com.br/api/v0/configs";
const char *queueRelay = "https://podoragreenhouse.com.br/api/v0/queuerelay";

// const String laravelBarear = "Bearer 1|48lKkFidFrs3DW1uTjCu5QlTFS8TE4OdoAhnTYOnaf769d55";
const String laravelBarear = "Bearer 5|zgepZCxDOcjJGuDbZ0JZq2jVWVlEk3QvbF2dUhNF0ee1566c";

// GSheets
//  #define PROJECT_ID "estufas-414918"
//  // Service Account's client email
//  #define CLIENT_EMAIL "valvulasestufas@estufas-414918.iam.gserviceaccount.com"
//  // Service Account's private key
//  const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDD04BqR9aTLWZ3\njNivjojqrHLVypjXFjiP1JIbyLotl4gP+HG0psoKufNmf78KuFidpeyY+DyLle5z\n0QI7+RHjXK/c4xTGdlhXCYlxhIGPtV9HGIuqGiO3BlRo35+EuG29EmlUXu71uCEb\nheLqz+ZjFFJfiCqdgEK7bp/id3SJmAJj3kqLjO8CRKDP4/XhBYMYfn+2doIv6CqG\nnaFrdOByGAAEy7QhJ/N+qRpXpN9vX95wEvK3aO4mwKQbxU7SMbhx7tV0n9B2/2mz\nwNmZSYFNOSieiCok7r2x9ovrACiE0YtgNgDi7VJsb3Xql6cDEwSFLHJzYSUypOpT\naE8sG4snAgMBAAECggEAB82gQtvs+X1oJWBn7Ar2er93bfx+Paflhhm23sOPY6yz\nn8tLIKM6ztWdFNsHTlKNOW74sBnIM9m8IEJWX/RrWY+CdH9Z/xSNLZ60KuFYVlUo\nYElim1siTDNE3s72aM/+k3et2c5UoFb/frzFj4zMRetz4tmmtRL81cIxPdF7w9XI\nahLrhq7oIcFAF2k87/TlIeUZjnPg+CufasbJ8MrHc0HGfImPXoGRzC36BBj2DAi0\n2l5M6koOr1l9eXAiG2lB4W/NYahPJw7c2PCJLphBHxdlkzNNqvFG+8otPa0DtTGu\nfH0D8Zx8jDEeRiLzm9j034DzRo6yHST9karNPisY7QKBgQD2GYRHNychAK/zH+1p\nwFoNM4nykMTDQtd8lce5x6Nnwzm/3E/1LuFcGQoiHnICjeSQmRb0f6K9p3XI8kD3\nGhhzNhAlp+3PY3HQL9tKgGsIyomBmUGdMu36WlQA72Y1aDtoDz0ehgjOs2O7pGej\n5UNvOaVECgor21a3BHGctv4oRQKBgQDLtDzwGyfrDjzo1Naa6SdC16ZUbtRbeMuD\nHMfqIbavn8x705Ysb+eSkbTooTVlunESO0B8nbnXJ2Snm0SSMD7JW0UYQJg90VVw\nXOSdpDjU0aVX2n+RFmByEhGB7f/ccl8s9JSoRNVnFMihf0RHqF3yeXix3cAwsYes\nQm3CyDWKewKBgQC1sCD4cr9wnhFIbUnZySiyllpBup+JiM+OKwZJIhqBgKAiBYUC\nfpniZJoWlmOVfyw/lMZdu91qJzfOIwvTYnCqnj9Nja0Eb2aXTL2ZQzXCR8U86OiI\nJhGLwyRBjHzo8QrqyFVXM51M/SOkZLwfRCOE+goJMWBRAJ4EgENgb7k0AQKBgAfC\n4UOAtV+mVjnEyjxyv7UzRDoZUuv9STvAn/lSuYQtgXwCTsl4pgkWlh767W7n0KES\nMt3MoKUK8jHUn6M2RV4kpRhMdcB9NlUmpOtAs42gQz16rLuZb6lI+PPLXqFGlku/\nMd7LaJoZ158Xm0Fjr9iqOigEu+rfCL1OdqIiQK/7AoGAN1B8A/SnY6LUNSEFyrur\nzuucXBMhfG6WBCs4tY2P35adfAlPg5TBUlyZc4zso7NR2xjxjb+EnqgrI4SsHiOT\n4YSNAfxRhugVm/w7z35/jFXQDTX6TNQ4TNCItvQe8itjpEXz1tVyutLUnRgAOTYL\nBVLlGGamAEIkO9/FPGA6A88=\n-----END PRIVATE KEY-----\n";
//  String GAS_ID = "AKfycbzBLOPGELlPC8_6D3ZIS8FH5wP_s_JMFfb9c_gjyyJ_9o1q5AF6JjzfJNAI3b_HJXVu";  //--> spreadsheet script ID