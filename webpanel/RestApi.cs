﻿using Nancy;
using Nancy.Helpers;
using Nancy.Responses;
using Newtonsoft.Json.Linq;
using NLog;
using SyberiaDatabase;
using System;
using System.IO;
using System.Text;

namespace SyberiaWebPanel
{
    public class RestApi : NancyModule
    {
        private static Logger logger = LogManager.GetCurrentClassLogger();

        public RestApi()
        {
            Get("/", x =>
            {
                var htmlPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", "login.html");
                return new GenericFileResponse(htmlPath, this.Context).NoCache();
            });

            Post("/login", x =>
            {
                var request = this.Context.Request;
                if (!WebPanel.GetInstance().CheckSpamFilter(request.UserHostAddress))
                {
                    logger.Warn($"[/login] Blocked by spam-filter. Too many failed login attempts from {request.UserHostAddress}");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.Forbidden
                    };
                }

                var body = new StreamReader(request.Body).ReadToEnd();
                var data = HttpUtility.ParseQueryString(body);
                var login = data.Get("login");
                var passMd5 = data.Get("pass");

                if (!WebPanel.GetInstance().CheckCredentials(login, passMd5))
                {
                    logger.Warn($"[/login] Invalid login from {request.UserHostAddress}. Spam filter incremented.");
                    WebPanel.GetInstance().IncrementSpamFilter(request.UserHostAddress);
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }

                WebPanel.GetInstance().ClearSpamFilter(request.UserHostAddress);

                var startTime = DateTime.Now;
                var gameConfig = WebPanel.GetInstance().GetGameConfig();
                var htmlPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", "main.html");
                var htmlData = new StringBuilder(File.ReadAllText(htmlPath));
                WebPanel.GetInstance().GetGameConfig().ReplaceValues(htmlData);

                var timeDiff = DateTime.Now - startTime;
                logger.Info($"[/login] Successfully logged in from {request.UserHostAddress}. UI rendered in {(int)timeDiff.TotalMilliseconds} ms.");
                return new TextResponse(htmlData.ToString(), "text/html", Encoding.UTF8).NoCache();
            });

            Post("/save", x => {
                var request = this.Context.Request;
                if (!WebPanel.GetInstance().CheckSpamFilter(request.UserHostAddress))
                {
                    logger.Warn($"[/login] Blocked by spam-filter. Too many failed login attempts from {request.UserHostAddress}");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.Forbidden
                    };
                }

                var bodyString = new StreamReader(request.Body).ReadToEnd();
                var bodyObject = Newtonsoft.Json.JsonConvert.DeserializeObject(bodyString) as JToken;
                var auth = bodyObject["auth"];
                var login = auth["login"].Value<string>();
                var passMd5 = auth["pass"].Value<string>();

                if (!WebPanel.GetInstance().CheckCredentials(login, passMd5))
                {
                    logger.Warn($"[/login] Invalid login from {request.UserHostAddress}. Spam filter incremented.");
                    WebPanel.GetInstance().IncrementSpamFilter(request.UserHostAddress);
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }

                var data = bodyObject["data"];
                WebPanel.GetInstance().GetGameConfig().Apply(data);
                WebPanel.GetInstance().GetGameConfig().Save();

                return new Response()
                {
                    StatusCode = HttpStatusCode.OK
                };
            });

            Post("/playersOffline", x => {
                var request = this.Context.Request;
                if (!WebPanel.GetInstance().CheckSpamFilter(request.UserHostAddress))
                {
                    logger.Warn($"[/login] Blocked by spam-filter. Too many failed login attempts from {request.UserHostAddress}");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.Forbidden
                    };
                }

                var body = new StreamReader(request.Body).ReadToEnd();
                var data = HttpUtility.ParseQueryString(body);
                var login = data.Get("login");
                var passMd5 = data.Get("pass");

                if (!WebPanel.GetInstance().CheckCredentials(login, passMd5))
                {
                    logger.Warn($"[/login] Invalid login from {request.UserHostAddress}. Spam filter incremented.");
                    WebPanel.GetInstance().IncrementSpamFilter(request.UserHostAddress);
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }

                var startTime = DateTime.Now;
                var htmlPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources", "players.html");
                var htmlData = new StringBuilder(File.ReadAllText(htmlPath));

                var database = DatabasePool.Instance.GetOrCreate("Syberia");
                htmlData.Replace("#{offlinePlayersList}", database.Query("SELECT uid, displayName, souls, respawnCounter, lastUpdate FROM characters"));
                htmlData.Replace("#{bansList}", Newtonsoft.Json.JsonConvert.SerializeObject(WebPanel.GetInstance().ReadBanList()));

                var timeDiff = DateTime.Now - startTime;
                logger.Info($"[/playersOffline] Successfully loaded list of offline players from {request.UserHostAddress}. UI rendered in {(int)timeDiff.TotalMilliseconds} ms.");
                return new TextResponse(htmlData.ToString(), "text/html", Encoding.UTF8).NoCache();
            });

            Post("/ban", x =>
            {
                var request = this.Context.Request;
                if (!WebPanel.GetInstance().CheckSpamFilter(request.UserHostAddress))
                {
                    logger.Warn($"[/login] Blocked by spam-filter. Too many failed login attempts from {request.UserHostAddress}");
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.Forbidden
                    };
                }

                var bodyString = new StreamReader(request.Body).ReadToEnd();
                var bodyObject = Newtonsoft.Json.JsonConvert.DeserializeObject(bodyString) as JToken;
                var auth = bodyObject["auth"];
                var login = auth["login"].Value<string>();
                var passMd5 = auth["pass"].Value<string>();

                if (!WebPanel.GetInstance().CheckCredentials(login, passMd5))
                {
                    logger.Warn($"[/login] Invalid login from {request.UserHostAddress}. Spam filter incremented.");
                    WebPanel.GetInstance().IncrementSpamFilter(request.UserHostAddress);
                    return new Response()
                    {
                        StatusCode = HttpStatusCode.InternalServerError
                    };
                }

                var uid = bodyObject["uid"].Value<string>();
                var ban = bodyObject["ban"].Value<bool>();
                WebPanel.GetInstance().ProccessBanUnban(uid, ban);

                return new Response()
                {
                    StatusCode = HttpStatusCode.OK
                };
            });
        }
    }
}