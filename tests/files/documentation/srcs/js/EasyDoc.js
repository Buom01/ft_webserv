/* eslint-disable no-unused-vars */

/**
 * EasyDoc.min.js contain full compressed code of projet
 * @summary	EasyDoc.min.js
 * @author	Clément Bertrand
 * @license	LGPLv3
 * @since	2021
 */

class CheckServicesAvailable
{
	static fetch()
	{
		if (window.fetch)
			return (true);
		return (false);
	}

	static localStorage()
	{
		try
		{
			const x = '__storage_test__';
			localStorage.setItem(x, x);
			localStorage.removeItem(x);
			return (true);
		}
		catch (e)
		{
			// eslint-disable-next-line no-undef
			return (e instanceof DOMException && (e.code === 22 || e.code === 1014 || e.name === 'QuotaExceededError' || e.name === 'NS_ERROR_DOM_QUOTA_REACHED') && (storage && storage.length !== 0));
		}
	}

	static cacheStorage()
	{
		const isCacheActive = 'caches' in window;
		if (isCacheActive)
			window.EasyDoc_IsCacheActive = true;
		else
			window.EasyDoc_IsCacheActive = false;
		return (window.EasyDoc_IsCacheActive);
	}
}

class EasyDocConfiguration
{
	constructor(config, updateOnlyMenu)
	{
		this._idOfDL = Number(0);
		if (typeof config !== 'undefined')
		{
			this._generate = config;
			if (typeof updateOnlyMenu !== 'undefined')
				this._generate.menu = this._menu(window.EasyDoc._menu).outerHTML;
		}
		else
		{
			this._generate = {
				title: null,
				favicon: null,
				logo: null,
				lang: {
					count: 0,
					html: null,
				},
				menu: null,
			};
			for (const id in window.EasyDoc._config)
				if (id === 'title_page')
					this._titlePage(window.EasyDoc._config, id);
				else if (id === 'favicon')
					this._favicon(window.EasyDoc._config, id);
				else if (id === 'logo')
					this._logo(window.EasyDoc._config, id);
				else if (id === 'lang')
					this._lang(window.EasyDoc._config[id]);
			this._generate.menu = this._menu(window.EasyDoc._menu).outerHTML;
		}
	}

	getConfig()
	{
		return (this._generate);
	}

	_apply(updateOnlyMenu)
	{
		if (typeof updateOnlyMenu === 'undefined')
		{
			if (this._generate.title)
				document.title = this._generate.title;
			if (this._generate.favicon)
			{
				const fav = document.createRange().createContextualFragment(this._generate.favicon);
				document.getElementsByTagName('head').item(0).appendChild(fav.firstChild);
			}
			if (this._generate.logo)
			{
				const img = document.querySelector('.easyDoc-sidenav-img');
				const newImg = document.createRange().createContextualFragment(this._generate.logo).firstChild;
				img.setAttribute('alt', newImg.getAttribute('alt'));
				img.setAttribute('src', newImg.getAttribute('src'));
			}
			if (this._generate.lang.html)
				if (this._generate.lang.count > 1)
				{
					const lang = document.createRange().createContextualFragment(this._generate.lang.html);
					document.querySelector('.easyDoc-sidenav-select').appendChild(lang);
				}
				else if (document.querySelector('.easyDoc-sidenav-select'))
				{
					document.querySelector('.easyDoc-sidenav-select').remove();
				}
		}
		if (this._generate.menu)
		{
			const list = document.querySelector('.easyDoc-sidenav-list');
			this._removeChildNodes(list);
			list.appendChild(document.createRange().createContextualFragment(this._generate.menu));
		}
		if (localStorage.getItem('EasyDoc._toogle'))
		{
			const tab = JSON.parse(localStorage.getItem('EasyDoc._toogle'));
			document.querySelectorAll('dl[dl-id]').forEach((el) =>
			{
				if (tab.indexOf(el.getAttribute('dl-id')) !== -1)
				{
					// eslint-disable-next-line no-param-reassign
					el.previousSibling.children[0].innerText = '▲';
					el.previousSibling.children[0].classList.add('arrow-animation-inverse');
					el.classList.add('dlHide');
				}
			});
		}
	}

	_removeChildNodes(parent)
	{
		while (parent.firstChild)
			parent.removeChild(parent.firstChild);
	}

	async _fetch(link)
	{
		return fetch(link).then((response) =>
		{
			if (!response.ok)
				throw Error(response.statusText);
			return (response.text());
		}).catch((error) =>
		{
			throw new Error(error);
		});
	}

	_titlePage(json, id)
	{
		this._generate.title = json[id];
	}

	_favicon(json, id)
	{
		const favicon = document.createElement('link');
		favicon.rel = 'icon';
		favicon.type = `image/${json[id].split('.').pop()}`;
		favicon.href = json[id];
		this._generate.favicon = favicon.outerHTML;
	}

	_logo(json, id)
	{
		const logo = document.createElement('img');
		logo.setAttribute('alt', 'menu_logo');
		logo.setAttribute('src', json[id]);
		this._generate.logo = logo.outerHTML;
	}

	_lang(json)
	{
		const div = document.createElement('div');
		for (const id in json)
			if (id !== 'default')
			{
				const element = document.createElement('option');
				element.setAttribute('value', id);
				element.innerText = json[id];
				div.appendChild(element);
				this._generate.lang.count++;
			}
		this._generate.lang.html = div.innerHTML;
	}

	_menu(json)
	{
		const DL = document.createElement('dl');
		DL.classList.add('innerDl');
		DL.setAttribute('dl-id', this._idOfDL++);
		let DT;
		for (const id in json)
			if (Object.prototype.toString.call(json[id]) === '[object Object]')
			{
				const arrow = document.createElement('span');
				arrow.classList.add('arrow-animation');
				arrow.appendChild(document.createTextNode('▼'));
				DT.appendChild(arrow);
				DL.appendChild(this._menu(json[id]));
			}
			else
			{
				DT = document.createElement('dt');
				DT.innerText = json[id][0]; // eslint-disable-line prefer-destructuring
				DT.setAttribute('data-link', json[id][1]);
				DL.appendChild(DT);
			}
		return (DL);
	}
}

class Hash
{
	static async sha1(data)
	{
		const msgBuffer = new TextEncoder('utf-8').encode(data);
		const hashBuffer = await crypto.subtle.digest('SHA-1', msgBuffer);
		const hashArray = Array.from(new Uint8Array(hashBuffer));
		const hashHex = hashArray.map((b) => b.toString(16).padStart(2, '0')).join('');
		return (hashHex);
	}

	static async sha256(data)
	{
		const msgBuffer = new TextEncoder('utf-8').encode(data);
		const hashBuffer = await crypto.subtle.digest('SHA-256', msgBuffer);
		const hashArray = Array.from(new Uint8Array(hashBuffer));
		const hashHex = hashArray.map((b) => b.toString(16).padStart(2, '0')).join('');
		return (hashHex);
	}

	static async sha384(data)
	{
		const msgBuffer = new TextEncoder('utf-8').encode(data);
		const hashBuffer = await crypto.subtle.digest('SHA-384', msgBuffer);
		const hashArray = Array.from(new Uint8Array(hashBuffer));
		const hashHex = hashArray.map((b) => b.toString(16).padStart(2, '0')).join('');
		return (hashHex);
	}

	static async sha512(data)
	{
		const msgBuffer = new TextEncoder('utf-8').encode(data);
		const hashBuffer = await crypto.subtle.digest('SHA-512', msgBuffer);
		const hashArray = Array.from(new Uint8Array(hashBuffer));
		const hashHex = hashArray.map((b) => b.toString(16).padStart(2, '0')).join('');
		return (hashHex);
	}

	static checksum(data)
	{
		return (this.sha256(data));
	}
}

class EasyDocMarkdown
{
	static open(input)
	{
		this.read(`${window.EasyDoc._url._href}${window.EasyDoc._config.directory.document}/${localStorage.getItem('EasyDoc._language')}/${input}`);
	}

	static read(link)
	{
		const _cache = (cache) =>
		{
			if (cache)
			{
				document.querySelector('.easyDoc-article-content').classList.remove('easyDoc-article-content-visible');
				document.querySelector('.easyDoc-loader').classList.add('easyDoc-loader-visible');
			}
			else
			{
				document.querySelector('.easyDoc-article-content').classList.add('easyDoc-article-content-visible');
				document.querySelector('.easyDoc-loader').classList.remove('easyDoc-loader-visible');
			}
		};
		_cache(true);
		fetch(link).then((response) =>
		{
			if (!response.ok)
				throw Error(response.statusText);
			return (response.text());
		}).then((data) =>
		{
			hljs.configure({ ignoreUnescapedHTML: true }); // eslint-disable-line no-undef
			const converter = window.markdownit({
				html: true,
				typographer: true,
				linkify: true,
				highlight: (str, lang) =>
				{
					if (lang && hljs.getLanguage(lang)) // eslint-disable-line no-undef
						try
						{
							return hljs.highlight(str, { language: lang, ignoreIllegals: true }).value; // eslint-disable-line no-undef
						}
						catch (__) {} // eslint-disable-line
					return '';
				},
			})
				.use(window.markdownitEmoji)
				.use(window.markdownitSub)
				.use(window.markdownitSup)
				.use(window.markdownitIns)
				.use(window.markdownitMark)
				.use(window.markdownitFootnote)
				.use(window.markdownitDeflist)
				.use(window.markdownitAbbr)
				.use(window.markdownItAnchor);

			document.querySelector('.easyDoc-article-content').innerHTML = converter.render(data);
			_cache(false);
			document.querySelector('.easyDoc-article-content').scrollTo({
				top: 0,
				left: 0,
				behavior: 'smooth',
			});
			try
			{
				hljs.highlightAll(); // eslint-disable-line no-undef
				const pageHash = new URL(document.location).hash;
				const docQuery = document.querySelector(pageHash);
				if (pageHash.length !== 0 && docQuery)
					document.querySelector('.easyDoc-article-content').scrollTo({
						top: Math.floor(docQuery.getBoundingClientRect().top),
						left: 0,
						behavior: 'smooth',
					});
			}
			catch (___) {} // eslint-disable-line
			document.querySelector('article').querySelectorAll('img').forEach((image) => image.addEventListener('click', () =>
			{
				const el = document.querySelector('.easyDoc-modal');
				el.classList.add('easyDoc-modal-visible');
				el.children[1].src = image.getAttribute('src');
			}));
		}).catch(() =>
		{
			this.read(`${window.EasyDoc._url._href}${window.EasyDoc._config.directory.document}/${localStorage.getItem('EasyDoc._language')}/404.md`);
		});
	}
}

class EasyDocUrl
{
	/*
	** URL : ?[_language]&[_page]#[_hash]
	*/
	constructor()
	{
		this._page_url = new URL(document.location);
		localStorage.setItem('EasyDoc._hostname', this._page_url.hostname);
		this._href = this._page_url.origin + this._page_url.pathname;
	}

	_parse()
	{
		this._page_url = new URL(document.location);
		const split = this._page_url.search.substring(1).split('&');
		return {
			lang: (split[0]) ? split[0] : null,
			file: (split[1]) ? split[1] : null,
			hash: (this._page_url.hash) ? this._page_url.hash : null,
		};
	}

	rewrite(defineLang = null, defineFile = null, defineHash = null, forceRemoveHash = false)
	{
		const args = this._parse();
		if (defineLang)
			args.lang = String(defineLang);
		if (defineFile)
			args.file = String(defineFile);
		if (defineHash)
			args.hash = String(defineHash);
		if (forceRemoveHash)
			args.hash = null;
		this._check(args);
		this._change(args);
		document.querySelectorAll('.easyDoc-sidenav-select > option').forEach((el) =>
		{
			if (el.getAttribute('value') === args.lang)
				el.setAttribute('selected', '');
		});
	}

	_parseMenu(arg)
	{
		const DTS = document.querySelectorAll('.easyDoc-sidenav-list dt');
		for (const DT of DTS)
		{
			const dataLink = DT.getAttribute('data-link').replace('.md', '');
			if (dataLink === arg)
				return dataLink;
		}
		return null;
	}

	_check(args)
	{
		let ifFoundLang = false;
		if (!args.lang && !args.file)
		{
			args.lang = String(window.EasyDoc._config.lang.default);
			args.file = String(window.EasyDoc._config.main_home[0].replace('.md', ''));
			args.hash = String('');
			localStorage.setItem('EasyDoc._file', args.file);
			return;
		}
		// Lang
		for (const id in window.EasyDoc._config.lang)
			if (args.lang === id)
			{
				ifFoundLang = true;
				break;
			}
		if (ifFoundLang === false)
			args.lang = window.EasyDoc._config.lang.default;
		localStorage.setItem('EasyDoc._language', args.lang);
		// File
		if (!args.file)
		{
			args.file = window.EasyDoc._config.main_home[0].replace('.md', '');
			args.hash = String('');
			localStorage.setItem('EasyDoc._file', args.file);
			return;
		}
		const parseFile = this._parseMenu(args.file);
		args.file = (!parseFile) ? '404' : parseFile;
		localStorage.setItem('EasyDoc._file', args.file);
	}

	_change(args)
	{
		const nextTitle = `${window.EasyDoc._config.title_page} | ${args.file}`;
		document.title = nextTitle;

		let nextURL = `${this._href}?${args.lang}&${args.file}`;
		if (args.hash !== null || ((typeof args.hash === 'string' || args.hash instanceof String) && args.hash.length > 0))
			nextURL += args.hash;
		if (window.EasyDoc.historyPop === false)
			window.history.pushState(
				JSON.stringify({
					EasyDoc: true,
					lang: args.lang,
					file: args.file,
					hash: args.hash,
				}),
				nextTitle,
				nextURL,
			);
		window.EasyDoc.historyPop = false;
	}
}

class EasyDoc
{
	constructor(configPath)
	{
		if (EasyDoc.exists)
			return (EasyDoc.instance); // eslint-disable-line no-constructor-return
		this._configPath = configPath;
		this._CacheActive = window.EasyDoc_IsCacheActive;
		delete window.EasyDoc_IsCacheActive;
		this._filesHash = {};
		this._config = {};
		this._menu = {};
		this._loader = null;
		this._url = new EasyDocUrl();
		if (!localStorage.getItem('EasyDoc._filesHash'))
			this._filesHash = { config: null, menu: null };
		else
			this._filesHash = { ...JSON.parse(localStorage.getItem('EasyDoc._filesHash')) };
		const fetch = async () =>
		{
			await this._configFile();
			await this._menuFile();
			localStorage.setItem('EasyDoc._filesHash', JSON.stringify(this._filesHash));
			this._url.rewrite();
			this.AddActiveLinkList(document.querySelectorAll('.easyDoc-sidenav-list dt'), true);
			EasyDocMarkdown.open(`${this._url._parse().file}.md`);
		}; fetch();
		EasyDoc.instance = this;
		EasyDoc.exists = true;
	}

	async _configFile()
	{
		const config 		= await this._fetch(this._configPath);
		const hashConfig	= await Hash.checksum(config).then((hash) => (hash));
		if (hashConfig && this._filesHash.config !== hashConfig)
		{
			this._filesHash.config = hashConfig;
			this._config = { ...JSON.parse(config) };
			localStorage.setItem('EasyDoc._language', this._config.lang.default);
			localStorage.setItem('EasyDoc._config', config);
		}
		else
		{
			this._config = { ...JSON.parse(localStorage.getItem('EasyDoc._config')) };
		}
	}

	async _menuFile(updateOnlyMenu)
	{
		let _configuration;
		const lang = String(this._url._parse().lang);
		if (lang)
			this.checkLang(lang);
		const linkMenu = `./${this._config.directory.document}/${localStorage.getItem('EasyDoc._language')}/menu.json`;
		const menu 			= await this._fetch(linkMenu);
		const hashMenu		= await Hash.checksum(menu).then((hash) => (hash));
		if (hashMenu && this._filesHash.menu !== hashMenu)
		{
			this._filesHash.menu = hashMenu;
			this._menu = { ...JSON.parse(menu) };
			localStorage.setItem('EasyDoc._menu', menu);
			_configuration = new EasyDocConfiguration();
			localStorage.setItem('EasyDoc._genConfig', JSON.stringify(_configuration.getConfig()));
		}
		else
		{
			this._menu = { ...JSON.parse(localStorage.getItem('EasyDoc._menu')) };
			if (typeof updateOnlyMenu !== 'undefined')
				_configuration = new EasyDocConfiguration(JSON.parse(localStorage.getItem('EasyDoc._genConfig'), true));
			else
				_configuration = new EasyDocConfiguration(JSON.parse(localStorage.getItem('EasyDoc._genConfig'), false));
		}
		if (typeof updateOnlyMenu !== 'undefined')
			_configuration._apply(true);
		else
			_configuration._apply();
	}

	async _fetch(link)
	{
		return fetch(link).then((response) =>
		{
			if (!response.ok)
				return ('{}');
			return (response.text());
		});
	}

	checkLang(lang)
	{
		let ifFoundLang = false;
		for (const id in window.EasyDoc._config.lang)
			if (lang === id)
			{
				ifFoundLang = true;
				break;
			}
		if (ifFoundLang === false)
			// eslint-disable-next-line no-param-reassign
			lang = window.EasyDoc._config.lang.default;
		localStorage.setItem('EasyDoc._language', lang);
	}

	_removeDlHide(element)
	{
		const parent = element.parentNode;
		if (parent.tagName === 'DL' && parent.classList.contains('dlHide'))
		{
			parent.classList.remove('dlHide');
			parent.previousSibling.children[0].innerText = '▼';
		}
		if (parent.getAttribute('dl-id'))
			this._removeDlHide(parent);
	}

	AddActiveLinkList(list, removeIfPass = false)
	{
		for (const element of list)
			if (element.getAttribute('data-link').replace('.md', '') === localStorage.getItem('EasyDoc._file'))
			{
				element.classList.add('activeLink');
				this._removeDlHide(element);
			}
			else if (removeIfPass === true)
			{
				element.classList.remove('activeLink');
			}
	}

	toogleMenu(element)
	{
		let isFound = false;
		const toogle = localStorage.getItem('EasyDoc._toogle') ? JSON.parse(localStorage.getItem('EasyDoc._toogle')) : [];
		for (const i in toogle)
			if (toogle[i] === element.getAttribute('dl-id'))
			{
				toogle.splice(i, 1);
				isFound = true;
				break;
			}
		if (!isFound)
			toogle.push(element.getAttribute('dl-id'));
		localStorage.setItem('EasyDoc._toogle', JSON.stringify(toogle));
	}
}

class Event
{
	constructor()
	{
		this._modal();
		this._select();
		this._sidenav();
		this._clickToLink();
		this._popstate();
	}

	_modal()
	{
		const temp = document.querySelector('.easyDoc-modal');
		const modal = {
			main: temp,
			span: temp.children[0],
		};
		modal.span.addEventListener('click', () => modal.main.classList.remove('easyDoc-modal-visible'));
		modal.main.addEventListener('click', (e) =>
		{
			if (e.target.classList.contains('easyDoc-modal'))
				modal.main.classList.remove('easyDoc-modal-visible');
		});
		document.addEventListener('keydown', (keydown) =>
		{
			if (keydown.key === 'Escape' && modal.main.classList.contains('easyDoc-modal-visible'))
				modal.main.classList.remove('easyDoc-modal-visible');
		});
	}

	_select()
	{
		document.querySelector('div.easyDoc-sidenav-icon > span').addEventListener('click', () => document.querySelector('.easyDoc-sidenav-menu').classList.toggle('active'));
		document.querySelector('.easyDoc-sidenav-select').addEventListener('input', (event) =>
		{
			const fetch = async () =>
			{
				localStorage.setItem('EasyDoc._language', event.target.value);
				window.EasyDoc._url.rewrite(event.target.value);
				await window.EasyDoc._menuFile(true);
				window.EasyDoc.AddActiveLinkList(document.querySelectorAll('.easyDoc-sidenav-list dt'));
				EasyDocMarkdown.open(`${window.EasyDoc._url._parse().file}.md`);
			}; fetch();
		}, false);
	}

	_sidenav()
	{
		document.querySelector('.easyDoc-sidenav-list').addEventListener('click', (event) =>
		{
			if (event.target.tagName === 'SPAN')
			{
				if (!event.target.classList.contains('arrow-animation-inverse'))
					event.target.innerText = '▲';
				else
					event.target.innerText = '▼';
				event.target.classList.toggle('arrow-animation-inverse');
				event.target.parentNode.nextSibling.classList.toggle('dlHide');
				window.EasyDoc.toogleMenu(event.target.parentNode.nextSibling);
				return;
			}
			if (event.target.tagName !== 'DT')
				return;
			const _reset = {
				dt: document.querySelectorAll('.easyDoc-sidenav-list dt'),
				arrow: document.querySelectorAll('.easyDoc-sidenav-list span'),
			};
			for (const element of _reset.dt)
				element.classList.remove('activeLink');
			for (const element of _reset.arrow)
				element.classList.remove('activeLink');
			event.target.classList.add('activeLink');
			const dataLink = event.target.getAttribute('data-link').replace('.md', '');
			localStorage.setItem('EasyDoc._file', dataLink);
			window.EasyDoc._url.rewrite(undefined, dataLink, undefined, true);
			EasyDocMarkdown.open(event.target.getAttribute('data-link'));
		}, false);
	}

	_clickToLink()
	{
		['click', 'wheel'].forEach((e) =>
		{
			window.addEventListener(e, (event) =>
			{
				if (event.target.tagName === 'A')
				{
					const href = event.target.getAttribute('href');
					try
					{
						if (!href || href[0] === '#')
							return;
						const url = new URL(href);
						if (url.hostname === localStorage.getItem('EasyDoc._hostname'))
							return;
						event.preventDefault();
						event.stopImmediatePropagation();
						window.open(href, '_blank');
					}
					catch (err)
					{
						// Link to another page ([page]#[hash])
						event.preventDefault();
						event.stopImmediatePropagation();
						const split = href.split('#');
						if (split.length === 1)
							window.EasyDoc._url.rewrite(undefined, split[0]);
						else
							window.EasyDoc._url.rewrite(undefined, split[0], `#${split[1]}`);
						window.EasyDoc.AddActiveLinkList(document.querySelectorAll('.easyDoc-sidenav-list dt'), true);
						EasyDocMarkdown.open(`${window.EasyDoc._url._parse().file}.md`);
					}
				}
			});
		});
	}

	_popstate()
	{
		window.addEventListener('popstate', (event) =>
		{
			if (event.state === null)
				return;
			const data = JSON.parse(event.state);
			if (Object.prototype.hasOwnProperty.call(data, 'EasyDoc'))
			{
				event.preventDefault();
				event.stopImmediatePropagation();
				window.EasyDoc.historyPop = true;
				window.EasyDoc._url.rewrite(event.state.lang, event.state.file, event.state.hash);
				window.EasyDoc.AddActiveLinkList(document.querySelectorAll('.easyDoc-sidenav-list dt'), true);
				EasyDocMarkdown.open(`${window.EasyDoc._url._parse().file}.md`);
			}
		});
	}
}

/**
 * Launch EasyDoc on your web page
 * @param {String} configPath Relative or absolute path of configuration file `config.json`
 * @returns Instance of EasyDoc, this instance is also available in `window.EasyDoc`
 */
function launchEasyDoc(configPath)
{
	if (!CheckServicesAvailable.fetch())
	{
		document.body.innerHTML = 'This site will not work on your browser, fetch request is not implemented. Update your browser';
		return (null);
	}
	if (!CheckServicesAvailable.cacheStorage())
		console.warn('Cache service is not present in your browser, files will not be cached correctly.');
	if (CheckServicesAvailable.localStorage())
	{
		window.EasyDoc = new EasyDoc(configPath);
		window.EasyDoc.Event = new Event();
		window.EasyDoc.historyPop = false;
		return (window.EasyDoc);
	}
	document.body.innerHTML = 'This site will not work on your browser, localstorage is not working.';
	return (null);
}
