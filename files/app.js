/* Integrated CampusConnect Frontend Controller */
const CC = (() => {
  const load = (k, d) => JSON.parse(localStorage.getItem(k) || JSON.stringify(d));
  const save = (k, v) => localStorage.setItem(k, JSON.stringify(v));
  
  let students = load('students', []);
  let accounts = load('accounts', {});
  let requests = load('requests', []);
  let chats = load('chats', []);

  const persist = () => {
    save('students', students);
    save('accounts', accounts);
    save('requests', requests);
    save('chats', chats);
  };

  const find = id => students.find(s => s.id === id);
  const err = m => { throw new Error(m); };

  function mergeSort(a) {
    if (a.length < 2) return a;
    const m = a.length >> 1, l = mergeSort(a.slice(0, m)), r = mergeSort(a.slice(m)), o = [];
    while (l.length && r.length) o.push(l[0].name <= r[0].name ? l.shift() : r.shift());
    return o.concat(l, r);
  }

  return {
    me: () => find(+sessionStorage.getItem('uid')),
    find,
    all: () => students,
    
    register(id, name, branch, year, pw) {
      if (!(id > 0)) err('Student ID must be positive.');
      if (accounts[id]) err('Account already exists. Please log in.');
      if (!name || !branch || year < 1 || year > 8) err('Fill all fields; year must be 1-8.');
      if (!pw || pw.length < 4 || /\s/.test(pw)) err('Password needs 4+ chars, no spaces.');
      if (!find(id)) students.push({ id, name, branch, year, skills: [], interests: [], connections: [] });
      accounts[id] = pw;
      persist();
    },

    deleteAccount(id) {
      const targetId = id || +sessionStorage.getItem('uid');
      students = students.filter(s => s.id !== targetId);
      delete accounts[targetId];
      requests = requests.filter(r => r.from !== targetId && r.to !== targetId);
      
      students.forEach(s => {
        s.connections = s.connections.filter(c => c !== targetId);
      });

      chats = chats.filter(c => c.senderId !== targetId && c.receiverId !== targetId);
      persist();
      if (targetId === +sessionStorage.getItem('uid')) {
        this.logout();
      }
    },

    removeConnection(targetId) {
      const m = this.me();
      if (!m) return;
      m.connections = m.connections.filter(id => id !== targetId);
      const other = find(targetId);
      if (other) {
        other.connections = other.connections.filter(id => id !== m.id);
      }
      persist();
    },

    sendMessage(receiverId, text) {
      const m = this.me();
      if (!text.trim()) err('Message text cannot be empty.');
      if (!m.connections.includes(receiverId)) err('You can only message accepted connections.');

      const messageObj = {
        senderId: m.id,
        receiverId: receiverId,
        text: text.trim(),
        timestamp: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
      };
      chats.push(messageObj);
      persist();
      return messageObj;
    },

    getChatHistory(otherId) {
      const m = this.me();
      return chats.filter(c => 
        (c.senderId === m.id && c.receiverId === otherId) || 
        (c.senderId === otherId && c.receiverId === m.id)
      );
    },

    login(id, pw) {
      if (accounts[id] !== pw) err('Incorrect student ID or password.');
      sessionStorage.setItem('uid', id);
    },
    
    logout() {
      sessionStorage.removeItem('uid');
      location.href = 'index.html';
    },

    add(kind, v) {
      v = v.trim();
      if (!v) err('Value cannot be empty.');
      const m = this.me();
      if (!m[kind].includes(v)) m[kind].push(v);
      persist();
    },

    search(type, key) {
      key = key.trim().toLowerCase();
      if (type === 'id') return students.filter(s => String(s.id) === key);
      if (type === 'branch') return students.filter(s => s.branch.toLowerCase() === key);
      return students.filter(s => s.skills.some(k => k.toLowerCase() === key));
    },

    sorted: () => mergeSort([...students]),

    send(to) {
      const m = this.me();
      if (!find(to)) err('Student ID not found.');
      if (to === m.id) err('You cannot connect with yourself.');
      if (m.connections.includes(to)) err('Already connected.');
      if (requests.some(r => (r.from === m.id && r.to === to) || (r.from === to && r.to === m.id))) {
        err('A request already exists.');
      }
      requests.push({ from: m.id, to });
      persist();
    },

    pending: () => requests.filter(r => r.to === +sessionStorage.getItem('uid')),

    process(accept) {
      const m = this.me(), i = requests.findIndex(r => r.to === m.id);
      if (i < 0) err('No pending requests.');
      const [r] = requests.splice(i, 1);
      if (accept) {
        const s = find(r.from);
        if (s) {
          s.connections.push(m.id);
          m.connections.push(s.id);
        }
      }
      persist();
    }
  };
})();
