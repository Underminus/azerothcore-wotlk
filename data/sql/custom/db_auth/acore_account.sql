-- Create account ACORE:ACORE
INSERT IGNORE INTO account (`id`, `username`, `salt`, `verifier`) VALUES (1, 'ACORE', X'eef1d1f029ee346e416a5d7734601c7536d26252196dc700585d84231d1b209d', X'5d255dd6ff4f9c6e31dd835ced89603f63b4dfdbfabd6aeabf62937441b3b37c');
-- Enable account GM access
INSERT IGNORE INTO account_access (`id`, `gmlevel`, `RealmID`) VALUES (1, 3, -1);
